/***
 * This code is a C++ implementation modified from an existing Python implementation from berndporr/py-ecg-detectors
 * which is a Python implementation of the QRS complex detection algorithm described in:
 *
 * Zong W, Moody G B, Jiang D.
 * A robust open-source algorithm to detect onset and duration of 
 * QRS complexes[C]//Computers in Cardiology, 2003. IEEE, 2003: 737-740.
 *
 * Apache License
 * Version 2.0, January 2004
 * http://www.apache.org/licenses/
 *
 * (C) 2024, Charles Chen <sduchenchao@gmail.com>
 *
 ***/

#ifndef WQRS_H
#define WQRS_H

#include <stdio.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>

class WqrsDetector {
public:
    WqrsDetector(float samplingRate) : fs(samplingRate) {}

    std::vector<int> detect(const std::vector<float>& unfilteredEcg) {
        std::vector<float> y = butterLowpassFilter(unfilteredEcg, 15);
        y = lengthTransform(y, static_cast<int>(std::ceil(fs * 0.13)));
        return threshold(y);
    }

private:
    float fs;

    std::vector<float> butterLowpassFilter(const std::vector<float>& data, float cutoff) {
        // Implementing a Butterworth lowpass filter
        // This function assumes you have a function or library to handle the filter coefficients and filtering
        // For the sake of simplicity, this will be a placeholder implementation
        std::vector<float> filteredData = data; // Placeholder
        // Call your butterworth filter implementation here
        // Example: filteredData = ButterworthFilter::apply(data, cutoff, fs);
        return filteredData;
    }

    std::vector<float> lengthTransform(const std::vector<float>& x, int w) {
        std::vector<float> l(w, 0.0f);
        for (size_t i = w; i < x.size(); ++i) {
            float ll = 0.0f;
            for (size_t j = i - w; j < i - 1; ++j) {
                ll += std::sqrt(std::pow(1.0f / fs, 2) + std::pow(x[j + 1] - x[j], 2));
            }
            l.push_back(ll);
        }
        return l;
    }

    std::vector<int> threshold(const std::vector<float>& x) {
        std::vector<int> peaks;
        std::vector<float> u = mwaConvolve(x, 10 * static_cast<int>(fs));
        for (size_t i = 0; i < x.size(); ++i) {
            if ((peaks.empty() || i > peaks.back() + static_cast<int>(fs * 0.35)) && x[i] > u[i]) {
                peaks.push_back(i);
            }
        }
        return peaks;
    }

    std::vector<float> mwaConvolve(const std::vector<float>& x, int windowSize) {
        std::vector<float> result(x.size(), 0.0f);
        for (size_t i = 0; i < x.size(); ++i) {
            float sum = 0.0f;
            int count = 0;
            for (int j = std::max(0, static_cast<int>(i) - windowSize / 2); j < std::min(static_cast<int>(x.size()), static_cast<int>(i) + windowSize / 2); ++j) {
                sum += x[j];
                count++;
            }
            result[i] = sum / count;
        }
        return result;
    }
};



#endif

