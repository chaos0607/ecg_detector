#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DetectorFactory.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <sample file> <detector type> <sample frequency>" << std::endl;
        std::cerr << "detector type: 0 for TwoAverage, 1 for Wqrs" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string input_file = argv[1];
    const int detector_type = std::stoi(argv[2]);
    const double fs = std::stod(argv[3]);

    std::ifstream finput(input_file);
    if (!finput.is_open()) {
        std::cerr << "Error opening file: " << input_file << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<double> unfiltered_ecg;
    double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

    while (finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z) {
        unfiltered_ecg.push_back(cs_V2_V1);
    }

    auto detector = DetectorFactory::createDetector(detector_type, fs);
    if (!detector) {
        std::cerr << "Invalid detector type: " << detector_type << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<int> rpeaks =  detector->detect(unfiltered_ecg);
    std::cout << "QRS detected at index:";
    for (int index : rpeaks) {
        std::cout << " " << index;
    }
    std::cout << std::endl;
    
    finput.close();
    return EXIT_SUCCESS;
}