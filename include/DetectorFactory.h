#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

#include "detectors/base.h"
#include "detectors/TwoAverage.h"
#include "detectors/wqrs.h"
#include "detectors/engzee.h"
#include <memory>
#include <iostream>

/**
 * @class DetectorFactory
 * @brief A factory class for creating ECG detectors.
 * 
 * This class provides a static method to create instances of different ECG
 * detector types based on a specified detector type identifier.
 */
class DetectorFactory {
public:
    /**
     * @brief Create a detector based on the specified type.
     * 
     * This static method creates and returns a unique pointer to an instance of a
     * specific detector type, determined by the `detector_type` parameter.
     * 
     * @param detector_type An integer representing the type of detector to create:
     * - 0: TwoAverageDetector
     * - 1: wqrsDetector
     * - 2: EngzeeDetector
     * @param fs The sampling frequency of the ECG signal.
     * @return A unique pointer to a BaseDetector instance of the specified type.
     */
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
