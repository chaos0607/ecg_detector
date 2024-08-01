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

/**
 * @brief The EngZee heartrate detector class
 * This class performs continuous heartrate detection on a stream of
 * ECG data.
 */
class EngzeeDetector : public BaseDetector {
public:
    /**
     * @brief Construct a new Engzee object
     * 
     * @param samplingrate The sampling rate in Hz in the region of 250Hz..300Hz.
     */
    EngzeeDetector(double samplingFrequency);
    
    std::vector<int> OfflineDetect(const std::vector<double>& unfiltered_ecg) override;

private:
    Fir lowhighpass;
    Fir MM;
    Fir past;

    int current_rpeak = 0;
    std::vector<int> rpeaks;
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

    void detect(float v);
};

#endif // ENGZEE_H