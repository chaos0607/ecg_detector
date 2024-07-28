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

#include <stdio.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "algorithms/fir.h"
#include "callback.h" 

/**
 * @brief The EngZee heartrate detector class
 * This class performs continous heartrate detection on a stream of
 * ECG data.
 */
class Engzee {
    public:
    std::vector<u_int> rpeaks;

/**
 * @brief Construct a new Engzee object
 * 
 * @param samplingrate The sampling rate in Hz in the region of 250Hz..300Hz.
 * @param callback An instancce of the HRCallback class
 */
    Engzee(float samplingrate, HRCallback& callback) : hrcallback(callback) {
        fs = samplingrate;
        ms200 = (int)(0.2*fs);
        ms1200 = (int)(1.2*fs);
        ms160 = (int)(0.16*fs);
        s2ctr = (int)(2*fs);
        neg_threshold = (int)(0.01*fs);
        past.init((int)(2*fs));
        lowhighpass.init({1,4,6,4,1,-1,-4,-6,-4,-1});
        MM.init(5);
        rpeaks.reserve(1000);
    }

    void detect(float v) {
        const float filtered = lowhighpass.filter(v);
        past.push(filtered);
        // threshold M
        if (s2ctr > 0) {
            // we are at the very start so thres is just estimated
            // from the most recent filtered samples
            M = 0.6f * past.max();
            MM.push(M);
            s2ctr --;
        } else if (haveQRS && (lastThresQRStimestamp < ms200)) {
            // The last QRS complex is less than 200ms away
            newM5 = 0.6f * past.max(lastThresQRStimestamp);
            if (newM5 > 1.5f * MM.buffer[1]) {
                newM5 = 1.1f * MM.buffer[1];
            }
        } else if (haveQRS && (newM5 > 0) && (lastThresQRStimestamp == ms200)) {
            // We are now exactly 200ms away take the newM5 value calc from the previous cond
            MM.push(newM5);
            M = MM.average();
        } else if (haveQRS && (lastThresQRStimestamp > ms200) && (lastThresQRStimestamp < ms1200)) {
            // We are now past the 200ms and lower the threshold at every timestep
            const float dy = 0.4f / (float)(ms1200 - ms200);
            float weighting = 1.0f - dy * (float)(lastThresQRStimestamp - ms200);
            M = MM.average() * weighting;
        } else if (haveQRS && (lastThresQRStimestamp >= ms1200)) {
            // We are now beyond 1.2sec and we keep the threshold at 0.6 of the maxima buffer
            M = 0.6f * MM.average();
        }

        if ((!haveQRS) && (filtered > M)) {
            // 1st QRS complex won't need a dead-time, just the threshold
            thi = true;
            haveQRS = true;
            lastThresQRStimestamp = 0;
        } else if ( haveQRS && (lastThresQRStimestamp > ms200) && (filtered > M) ) {
            // following QRS complexes should only be detected after a dead-time of 200ms
            thi = true;
            haveQRS = true;
            lastThresQRStimestamp = 0;
        }

        if (thi && (lastThresQRStimestamp < ms160)) {
            // detecting the neg slope diving through the neg thresold
            if ((past.buffer[0] < -M) && (past.buffer[1] > -M)) {
                thf = true;
            }
            if (thf && (filtered < -M)) {
                // keeping counting as long as it's negative
                counter++;
            } else if ((filtered > -M) && thf) {
                // resetting the counter once it re-surfaces
                counter = 0;
                thi = false;
                thf = false;
            }
        } else if (thi && (lastThresQRStimestamp > ms160)) {
            // all is lost
            counter = 0;
            thi = false;
            thf = false;
        }

        if (counter > neg_threshold) {
            float max = 0.0;
            int index = 0;
            for(int i = 0; 
                ( i < (lastThresQRStimestamp + neg_threshold) ) && ( i < (int)past.buffer.size() ); 
                i++) {
                if (past.buffer[i] > max) {
                    index = i;
                    max = past.buffer[i];
                }
            }
            if (!firstDetection) {
                float dSamples = (float)(lastRelativeQRStimestamp - index);
                
                current_rpeak = current_rpeak + lastRelativeQRStimestamp - index; //need to check why result is always plus 1 as the original python result
                rpeaks.push_back(current_rpeak);
                printf("current_rpeak = %d\n",current_rpeak);

                lastRelativeQRStimestamp = index;
                float hr = 60*fs / dSamples;
                hrcallback.hasHR(hr);
            }
            firstDetection = false;
            counter = 0;
            thi = false;
            thf = false;
        }

        lastThresQRStimestamp++;
        lastRelativeQRStimestamp++;
    }

    private:

    Fir lowhighpass;
    Fir MM;
    Fir past;

    int current_rpeak = 0;
    int ms200;
    int ms1200;
    int ms160;
    int neg_threshold;
    float fs;
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
    HRCallback& hrcallback;
};

#endif
