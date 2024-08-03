#ifndef ECGDETECTOR_H
#define ECGDETECTOR_H

#include <vector>
#include <string>
#include <stdexcept>

/**
 * @class EcgDetector
 * @brief A utility class for performing offline ECG signal detection.
 * 
 * This class provides static methods for detecting R-peaks in ECG signals
 * using various detection algorithms. The detection can be performed on
 * raw ECG data or data loaded from a file.
 */
class EcgDetector {
public:

    /**
     * @brief Perform offline QRS detection on unfiltered ECG data.
     * 
     * This method processes an ECG signal and detects R-peaks using a specified
     * detector algorithm.
     * 
     * @param unfiltered_ecg A vector containing the unfiltered ECG signal data.
     * @param detector_type An integer specifying the type of detector to use:
     * - 0: TwoAverageDetector
     * - 1: wqrsDetector
     * - 2: EngzeeDetector
     * @param fs The sampling frequency of the ECG signal.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     */
    static std::vector<int> offlinedetect(const std::vector<double>& unfiltered_ecg, int detector_type, double fs);

    /**
     * @brief Perform offline QRS detection from an ECG data file.
     * 
     * This method reads ECG signal data from a file, processes it, and detects R-peaks
     * using a specified detector algorithm.
     * 
     * @param filename The name of the file containing the ECG data.
     * @param detector_type An integer specifying the type of detector to use:
     * - 0: TwoAverageDetector
     * - 1: wqrsDetector
     * - 2: EngzeeDetector
     * @param fs The sampling frequency of the ECG signal.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     * 
     * @throws std::runtime_error if the file cannot be opened or read.
     */
    static std::vector<int> offlinedetectFromFile(const std::string& filename, int detector_type, double fs);
};

#endif // ECGDETECTOR_H
