#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <functional>
#include "iir/Butterworth.h"
#include "detectors/TwoAverage.h"
#include "algorithms/mwa.h"
#include <fstream>  

//use Forward and Reverse Filtering to decrease the Phase Delay of the signal
void decreaseDelayReverseFiltering(std::vector<double>& signal, Iir::Butterworth::BandPass<2>& filter) {
    std::reverse(signal.begin(), signal.end());
    for (size_t i = 0; i < signal.size(); ++i) {
        signal[i] = filter.filter(signal[i]);
    }
    std::reverse(signal.begin(), signal.end());
}

void decreaseDelayAddCompensation(std::vector<double>& signal, int delaySamples) {
    if (delaySamples < 0 || delaySamples >= static_cast<int>(signal.size())) {
        std::cerr << "Invalid delaySamples value." << std::endl;
        return;
    }
    std::rotate(signal.begin(), signal.begin() + delaySamples, signal.end());
    std::fill(signal.begin(), signal.begin() + delaySamples, signal[delaySamples]);
}


std::vector<int> TwoAverageDetector::OfflineDetect(const std::vector<double>& unfiltered_ecg) {

    Iir::Butterworth::BandPass<2> iirbandpass;
	iirbandpass.setup(samplingFrequency, 14, 6);

    std::vector<double> filtered_ecg;
    filtered_ecg.reserve(unfiltered_ecg.size());

    for (const auto& sample : unfiltered_ecg) {
        filtered_ecg.push_back(iirbandpass.filter(sample));
    }


    // use to decrease the Phase Delay of the signal
    //decreaseDelayReverseFiltering(filtered_ecg, iirbandpass);
    decreaseDelayAddCompensation(filtered_ecg, 16);
    //saveEcgDataToFile(filtered_ecg, "filtered_ecg.txt");

    std::vector<double> abs_filtered_ecg(filtered_ecg.size());
    std::transform(filtered_ecg.begin(), filtered_ecg.end(), abs_filtered_ecg.begin(), [](double val) {
        return std::abs(val);
    });

    size_t window1 = static_cast<size_t>(0.12 * samplingFrequency);
    std::vector<double> mwa_qrs = MWA_cumulative(abs_filtered_ecg, window1);

    size_t window2 = static_cast<size_t>(0.6 * samplingFrequency);
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

    for (int i = 1; i < static_cast<int>(blocks.size()); ++i) { 
        if (blocks[i - 1] == 0.0 && blocks[i] == block_height) {  
            start = i;
        } else if (blocks[i - 1] == block_height && blocks[i] == 0.0) {
            end = i - 1;
            if (end - start > static_cast<int>(0.08 * samplingFrequency)) {
                int detection = static_cast<int>(std::distance(filtered_ecg.begin(), std::max_element(filtered_ecg.begin() + start, filtered_ecg.begin() + end + 1)));
                if (!QRS.empty()) {
                    if (detection - QRS.back() > static_cast<int>(0.3 * samplingFrequency)) {
                        //std::cout << "QRS detected at index: " << detection << std::endl;
                        QRS.push_back(detection);
                    }
                } else {
                    //std::cout << "QRS detected at index: " << detection << std::endl;
                    QRS.push_back(detection);
                }
            }
        }
    }

    return QRS;
}