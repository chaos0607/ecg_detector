/***
 * C++ implementation of 
 *
 * W Zong, GB Moody, D Jiang 
 * A Robust Open-source Algorithm to Detect Onset and Duration of QRSComplexes 
 * 2003 IEEE
 *
 * Apache License
 * Version 2.0, January 2004
 * http://www.apache.org/licenses/
 *
 * (C) 2024, Charles Chen
 *
 ***/
#ifndef WQRS_H
#define WQRS_H

#include <vector>
#include "base.h"

/**
 * @class wqrsDetector
 * @brief A QRS detector implementing the length transform method.
 * 
 * This class detects QRS complexes in ECG signals using the wavelet transform 
 * and length transformation techniques.
 */
class wqrsDetector : public BaseDetector {
public:
    /**
     * @brief Construct a new wqrsDetector object.
     * 
     * @param samplingFrequency The sampling frequency of the ECG signal.
     */
    wqrsDetector(double samplingFrequency) : BaseDetector(samplingFrequency) {}

    /**
     * @brief Perform length transform on the ECG signal.
     * 
     * This method applies a length transformation to the ECG signal for feature enhancement.
     * 
     * @param x The input ECG signal data.
     * @param w The window size for the transformation.
     * @param fs The sampling frequency of the signal.
     * @return A vector containing the transformed ECG signal.
     */
    std::vector<double> lengthTransform(const std::vector<double>& x, int w, double fs);

    /**
     * @brief Apply thresholding to the transformed signal.
     * 
     * This method applies a threshold to detect potential QRS complexes in the transformed signal.
     * 
     * @param x The transformed ECG signal data.
     * @param fs The sampling frequency of the signal.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     */
    std::vector<int> threshold(const std::vector<double>& x, double fs);

    /**
     * @brief Perform offline QRS detection using the wqrs method.
     * 
     * This method processes the entire ECG signal and detects R-peaks using the length transform
     * and thresholding techniques.
     * 
     * @param unfiltered_ecg A vector containing the unfiltered ECG signal data.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     */
    std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) override;
};

#endif // WQRS_H
