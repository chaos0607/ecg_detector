#ifndef BASEDETECTOR_H
#define BASEDETECTOR_H

#include <vector>

/**
 * @class BaseDetector
 * @brief An abstract base class for ECG signal detection.
 *
 * This class serves as a base for different ECG detection algorithms, providing a
 * common interface and storing the sampling frequency of the signal.
 */
class BaseDetector {
public:
    /**
     * @brief Constructs a BaseDetector object with a specified sampling frequency.
     * 
     * @param samplingFrequency The sampling frequency of the ECG signal.
     */
    BaseDetector(double samplingFrequency) : samplingFrequency(samplingFrequency) {}

    /**
     * @brief Virtual destructor for BaseDetector.
     */
    virtual ~BaseDetector() = default;

    /**
     * @brief Pure virtual function for offline detection of ECG signals.
     * 
     * This function must be implemented by derived classes to perform offline
     * detection on unfiltered ECG data.
     * 
     * @param unfiltered_ecg A vector containing the unfiltered ECG signal data.
     * @return A vector of integers representing detected events or features in the ECG signal.
     */
    virtual std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) = 0;

protected:
    /**
     * @brief The sampling frequency of the ECG signal.
     */
    double samplingFrequency;
};

#endif // BASEDETECTOR_H
