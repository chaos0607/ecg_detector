// onlineData.h
#ifndef ONLINEDATA_H
#define ONLINEDATA_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "detectors/engzee.h"
#include <fstream>

struct DataPoint {
    double ecg_data;
    float heart_rate;
};

class OnlineData {
private:
    std::queue<DataPoint>& data_queue;
    std::mutex& mtx;
    std::condition_variable& cv;
    EngzeeDetector detector;
    double samplingFrequency;
    std::string filename;
    std::ifstream finput;

    DataPoint getNewdata() {
        double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

        finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z;
        float heartRate = detector.OnlineDetect(cs_V2_V1);
        return DataPoint{cs_V2_V1, heartRate};
        
    }

public:
    OnlineData(std::queue<DataPoint>& dq, std::mutex& m, std::condition_variable& c, double fs, std::string path)
        : data_queue(dq), mtx(m), cv(c), detector(fs), samplingFrequency(fs), filename(path) {
        finput.open(filename);
        if (!finput.is_open()) {
            std::cerr << "Error: fail to open file: " << filename << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void generate_data() {
        while (true) {
            DataPoint new_data;
            
            new_data = getNewdata();
            {
                std::lock_guard<std::mutex> lock(mtx);
                data_queue.push(new_data);
            }
            cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(4)); // 4ms, 250Hz
        }
    }
};

#endif // ONLINEDATA_H