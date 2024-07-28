#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <functional>
#include "iir/Butterworth.h"
#include "detectors/TwoAverage.h"
#include "detectors/common.h"
#include <fstream>  

TwoAverageDetector::TwoAverageDetector(double fs) : fs(fs) {
}

std::vector<int> TwoAverageDetector::detect(const std::vector<double>& unfiltered_ecg) {

    Iir::Butterworth::BandPass<2> iirbandpass ;
	iirbandpass.setup(fs, 14, 6);

    std::vector<double> filtered_ecg;
    filtered_ecg.reserve(unfiltered_ecg.size());

    for (const auto& sample : unfiltered_ecg) {
        filtered_ecg.push_back(iirbandpass.filter(sample));
    }


    // use to decrease the Phase Delay of the signal
    std::reverse(filtered_ecg.begin(), filtered_ecg.end());
    for (size_t i = 0; i < filtered_ecg.size(); ++i) {
        filtered_ecg[i] = iirbandpass.filter(filtered_ecg[i]);
    }
    std::reverse(filtered_ecg.begin(), filtered_ecg.end());

    //saveEcgDataToFile(filtered_ecg, "filtered_ecg.txt");

    std::vector<double> abs_filtered_ecg(filtered_ecg.size());
    std::transform(filtered_ecg.begin(), filtered_ecg.end(), abs_filtered_ecg.begin(), [](double val) {
        return std::abs(val);
    });

    int window1 = static_cast<int>(0.12 * fs);
    std::vector<double> mwa_qrs = MWA_cumulative(abs_filtered_ecg, window1);

    int window2 = static_cast<int>(0.6 * fs);
    std::vector<double> mwa_beat = MWA_cumulative(abs_filtered_ecg, window2);

    std::vector<double> blocks(unfiltered_ecg.size(), 0.0);
    double block_height = *std::max_element(filtered_ecg.begin(), filtered_ecg.end());
    for (size_t i = 0; i < mwa_qrs.size(); ++i) {
        if (mwa_qrs[i] > mwa_beat[i]) {
            blocks[i] = block_height;
        } else {
            blocks[i] = 0.0;
        }
    }

    std::vector<int> QRS;
    int start = 0, end = 0;

    for (size_t i = 1; i < blocks.size(); ++i) { 
        if (blocks[i - 1] == 0.0 && blocks[i] == block_height) {  
            start = i;
        } else if (blocks[i - 1] == block_height && blocks[i] == 0.0) {
            end = i - 1;
            if (end - start > static_cast<int>(0.08 * fs)) {
                int detection = std::distance(filtered_ecg.begin(), std::max_element(filtered_ecg.begin() + start, filtered_ecg.begin() + end + 1));
                if (!QRS.empty()) {
                    if (detection - QRS.back() > static_cast<int>(0.3 * fs)) {
                        std::cout << "QRS detected at index: " << detection << std::endl;
                        QRS.push_back(detection);
                    }
                } else {
                    std::cout << "QRS detected at index: " << detection << std::endl;
                    QRS.push_back(detection);
                }
            }
        }
    }

    return QRS;
}