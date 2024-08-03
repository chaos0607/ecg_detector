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

/**
 * @struct DataPoint
 * @brief A structure to hold ECG data and calculated heart rate.
 */
struct DataPoint {
    double ecg_data; ///< ECG data value.
    float heart_rate; ///< Calculated heart rate.
};

/**
 * @class OnlineData
 * @brief Class for handling online ECG data processing.
 * 
 * This class reads ECG data from a file, processes it in real-time to calculate heart rate, 
 * and stores the results in a thread-safe queue.
 */
class OnlineData {
private:
    std::queue<DataPoint>& data_queue; ///< Queue for storing processed data points.
    std::mutex& mtx;
    std::condition_variable& cv;
    EngzeeDetector detector; ///< Detector for processing ECG data and calculating heart rate.
    double samplingFrequency;
    std::string filename;
    std::ifstream finput;

    /**
     * @brief Read new ECG data from the input file and process it.
     * 
     * This method reads a new line of ECG data from the file, uses the detector to calculate
     * the heart rate, and returns the processed data as a DataPoint.
     * 
     * @return A DataPoint containing the latest ECG data and calculated heart rate.
     */
    DataPoint getNewdata() {
        double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

        finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z;
        float heartRate = detector.OnlineDetect(cs_V2_V1);
        return DataPoint{cs_V2_V1, heartRate};
        
    }

public:
    /**
     * @brief Construct a new OnlineData object.
     * 
     * Initializes the detector, opens the input file, and sets up the queue and synchronization primitives.
     * 
     * @param dq Reference to the data queue.
     * @param m Reference to the mutex for queue synchronization.
     * @param c Reference to the condition variable for queue notifications.
     * @param fs The sampling frequency of the ECG data.
     * @param path Path to the input file containing ECG data.
     */
    OnlineData(std::queue<DataPoint>& dq, std::mutex& m, std::condition_variable& c, double fs, std::string path)
        : data_queue(dq), mtx(m), cv(c), detector(fs), samplingFrequency(fs), filename(path) {
        finput.open(filename);
        if (!finput.is_open()) {
            std::cerr << "Error: fail to open file: " << filename << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    /**
     * @brief Continuously generate and process ECG data.
     * 
     * This method runs an infinite loop that reads, processes, and enqueues ECG data points.
     * Each processed data point is added to the queue, and a notification is sent to waiting threads.
     */
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
