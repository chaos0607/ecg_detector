#ifndef TWOAVERAGEDETECTOR_H
#define TWOAVERAGEDETECTOR_H

#include <vector>
#include "base.h"

class TwoAverageDetector : public BaseDetector {
public:
    TwoAverageDetector(double fs) : BaseDetector(fs) {}
    std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) override;
};

#endif // TWOAVERAGEDETECTOR_H