/***
 * C++ implementation of 
 *
 * W. Engelse and C. Zeelenberg, “A single scan algorithm for QRS detection 
 * and feature extraction”, IEEE Comp. in Cardiology, vol. 6, pp. 37-42, 1979 
 * with modifications A. Lourenco, H. Silva, P. Leite, R. Lourenco and A. Fred,
 * “Real Time Electrocardiogram Segmentation for Finger Based ECG Biometrics”, 
 * BIOSIGNALS 2012, pp. 49-54, 2012.
 *
 * Apache License
 * Version 2.0, January 2004
 * http://www.apache.org/licenses/
 *
 * (C) 2022, Bernd Porr <mail@berndporr.me.uk>
 *
 ***/
#ifndef ENGZEE_H
#define ENGZEE_H

#include <vector>
#include "algorithms/fir.h"
#include "base.h"
#include "callback.h"
#include "iir/Butterworth.h"

/**
 * @class EngzeeDetector
 * @brief The Engzee heart rate detector class.
 * 
 * This class performs continuous heart rate detection on a stream of
 * ECG data using a combination of FIR and IIR filters.
 */
class EngzeeDetector : public BaseDetector {
public:
    /**
     * @brief Construct a new EngzeeDetector object.
     * 
     * @param samplingFrequency The sampling rate in Hz, typically in the range of 250Hz to 300Hz.
     */
    EngzeeDetector(double samplingFrequency);
    
    /**
     * @brief Perform offline QRS detection on unfiltered ECG data.
     * 
     * This method processes the entire ECG signal and detects R-peaks.
     * 
     * @param unfiltered_ecg A vector containing the unfiltered ECG signal data.
     * @return A vector of indices representing detected R-peaks in the ECG signal.
     */
    std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) override;

    /**
     * @brief Perform online QRS detection on a single ECG sample.
     * 
     * This method processes ECG data in real-time, detecting R-peaks from a continuous input.
     * 
     * @param unfiltered_ecg A single sample of unfiltered ECG data.
     * @return A float indicating detection status or amplitude of detected R-peak.
     */
    float OnlineDetect(const double unfiltered_ecg);

private:
    Fir lowhighpass; ///< Low-high pass filter for signal preprocessing.
    Fir MM; ///< Moving mean filter for baseline correction.
    Fir past; ///< FIR filter for past signal data.

    int current_rpeak = 0; ///< Index of the current R-peak.
    std::vector<int> rpeaks; ///< Vector storing indices of detected R-peaks.
    int cutoff;
    int ms200;
    int ms1200;
    int ms160;
    int neg_threshold;
    int s2ctr;
    float M = 0;
    int lastThresQRStimestamp = 0;
    int currentQRStimestamp = 0;
    bool haveQRS = false;
    float newM5 = 0;
    bool thi = false;
    bool thf = false;
    int counter = 0;
    int lastRelativeQRStimestamp = 0;
    bool firstDetection = true;
    Iir::Butterworth::BandStop<2> iirnotch; ///< Bandstop IIR filter to remove powerline interference.

    /**
     * @brief Perform QRS detection on a given sample.
     * 
     * @param v The input sample value.
     * @param online Flag indicating if detection is in online mode.
     * @return A float indicating detection status or feature.
     */
    float detect(float v, bool online);
};

#endif // ENGZEE_H
