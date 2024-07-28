#ifndef BASEDETECTOR_H
#define BASEDETECTOR_H

#include <vector>

class BaseDetector {
public:
    BaseDetector(double fs) : fs(fs) {}
    virtual ~BaseDetector() = default;

    virtual std::vector<int> detect(const std::vector<double>& unfiltered_ecg) = 0;

protected:
    double fs;
};

#endif // BASEDETECTOR_H