#include <iostream>
#include <vector>
#include <fstream>
#include "EcgDetector.h"
#include "DetectorFactory.h"


std::vector<int> EcgDetector::offlinedetect(const std::vector<double>& unfiltered_ecg, int detector_type, double fs) {
    auto detector = DetectorFactory::createDetector(detector_type, fs);
    if (!detector) {
        throw std::invalid_argument("Invalid detector type: " + std::to_string(detector_type));
    }

    std::vector<int> rpeaks =  detector->detect(unfiltered_ecg);
    return rpeaks;
}

std::vector<int> EcgDetector::offlinedetectFromFile(const std::string& filename, int detector_type, double fs) {
    std::ifstream finput(filename);
    if (!finput.is_open()) {
        throw std::invalid_argument("fail to open file: " + filename);
    }

    std::vector<double> unfiltered_ecg;
    double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

    while (finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z) {
        unfiltered_ecg.push_back(cs_V2_V1);
    }

    std::vector<int> rpeaks = offlinedetect(unfiltered_ecg, detector_type, fs);
    return rpeaks;
}