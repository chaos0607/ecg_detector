#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

#include "detectors/base.h"
#include "detectors/TwoAverage.h"
#include "detectors/wqrs.h"
#include <memory>

class DetectorFactory {
public:
    static std::unique_ptr<BaseDetector> createDetector(int detector_type, double fs) {
        switch (detector_type) {
            case 0:
                return std::make_unique<TwoAverageDetector>(fs);
            case 1:
                return std::make_unique<wqrsDetector>(fs);
            default:
                return nullptr;
        }
    }
};

#endif // DETECTORFACTORY_H