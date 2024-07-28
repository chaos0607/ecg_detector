#ifndef WQRS_H
#define WQRS_H

#include <vector>
#include "base.h"

class wqrsDetector : public BaseDetector {
public:
    wqrsDetector(double fs) : BaseDetector(fs) {}

    std::vector<double> lengthTransform(const std::vector<double>& x, int w, double fs);
    std::vector<int> threshold(const std::vector<double>& x, double fs);
    std::vector<int> detect(const std::vector<double>& unfiltered_ecg) override;
};

#endif // WQRS_H