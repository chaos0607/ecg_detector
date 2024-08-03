/***
 * C++ implementation of 
 *
 * Elgendi, Mohamed & Jonkman, Mirjam & De Boer, Friso. (2010).
 * Frequency Bands Effects on QRS Detection.
 * The 3rd International Conference on Bio-inspired Systems 
 * and Signal Processing (BIOSIGNALS2010). 428-431.
 *
 * Apache License
 * Version 2.0, January 2004
 * http://www.apache.org/licenses/
 *
 * (C) 2024, Charles Chen
 *
 ***/
#ifndef TWOAVERAGEDETECTOR_H
#define TWOAVERAGEDETECTOR_H

#include <vector>
#include "base.h"

/**
 * @class TwoAverageDetector
 * @brief A QRS detector using the two average method.
 * 
 * This class implements a QRS detection algorithm based on computing two moving averages
 * to identify the R-peaks in ECG signals.
 */
class TwoAverageDetector : public BaseDetector {
public:
    /**
     * @brief Construct a new TwoAverageDetector object.
     * 
     * @param samplingFrequency The sampling frequency of the ECG signal.
     */
    TwoAverageDetector(double samplingFrequency) : BaseDetector(samplingFrequency) {}

    /**
     * @brief Perform offline QRS detection using the two average method.
     * 
     * This method analyzes the entire ECG signal and detects R-peaks by comparing
     * two moving averages.
     * 
     * @param unfiltered_ecg A vector containing the unfiltered ECG signal data.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     */
    std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) override;
};

#endif // TWOAVERAGEDETECTOR_H
