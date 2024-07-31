#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>
#include "iir/Butterworth.h"
#include "detectors/wqrs.h"
#include "algorithms/mwa.h"

std::vector<double> wqrsDetector::lengthTransform(const std::vector<double>& x, int w, double fs) {
    std::vector<double> tmp;
    for (size_t i = w; i < x.size(); ++i) {
        std::vector<double> chunk(x.begin() + i - w, x.begin() + i);
        double ll = 0.0;
        for (size_t j = 1; j < chunk.size(); ++j) {
            ll += std::sqrt(std::pow(1.0 / fs, 2) + std::pow(chunk[j] - chunk[j - 1], 2));
        }
        tmp.push_back(ll);
    }
    std::vector<double> l(w, tmp[0]);
    l.insert(l.end(), tmp.begin(), tmp.end());
    return l;
}

std::vector<int> wqrsDetector::threshold(const std::vector<double>& x, double fs) {
    std::vector<int> peaks;
    std::vector<double> u = MWA_convolve(x, static_cast<size_t>(10 * fs));
    for (int i = 0; i < static_cast<int>(x.size()); ++i) {
        if ((peaks.empty() || i > peaks.back() + static_cast<int>(fs * 0.35)) && x[i] > u[i]) {
            peaks.push_back(i);
        }
    }
    return peaks;
}

std::vector<int> wqrsDetector::OfflineDetect(const std::vector<double>& unfiltered_ecg) {
    
    Iir::Butterworth::LowPass<2> iirlowpass ;
	iirlowpass.setup(samplingFrequency, 15);

    std::vector<double> filtered_ecg;
    filtered_ecg.reserve(unfiltered_ecg.size());

    for (const auto& sample : unfiltered_ecg) {
        filtered_ecg.push_back(iirlowpass.filter(sample));
    }

    filtered_ecg = lengthTransform(filtered_ecg, static_cast<int>(std::ceil(samplingFrequency * 0.13)), samplingFrequency);
    return threshold(filtered_ecg, samplingFrequency);
}