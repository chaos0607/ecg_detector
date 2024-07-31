#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

#include "detectors/base.h"
#include "detectors/TwoAverage.h"
#include "detectors/wqrs.h"
#include "detectors/engzee.h"
#include <memory>

class DetectorFactory {
public:
    static std::unique_ptr<BaseDetector> createDetector(int detector_type, double fs) {
        switch (detector_type) {
            case 0:
                return std::make_unique<TwoAverageDetector>(fs);
            case 1:
                return std::make_unique<wqrsDetector>(fs);
            case 2:
                return std::make_unique<EngzeeDetector>(fs);
            default:
                std::cerr << "Error: Unknown detector type!" << std::endl;
                std::exit(EXIT_FAILURE);
        }
    }
};

#endif // DETECTORFACTORY_H