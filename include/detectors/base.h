#ifndef BASEDETECTOR_H
#define BASEDETECTOR_H

#include <vector>

class BaseDetector {
public:
    BaseDetector(double samplingFrequency) : samplingFrequency(samplingFrequency) {}
    virtual ~BaseDetector() = default;

    virtual std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) = 0;

protected:
    double samplingFrequency;
};

#endif // BASEDETECTOR_H