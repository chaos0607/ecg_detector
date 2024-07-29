#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "EcgDetector.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <sample file> <detector type> <sample frequency>" << std::endl;
        std::cerr << "detector type: 0 for TwoAverage, 1 for Wqrs" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string input_file = argv[1];
    const int detector_type = std::stoi(argv[2]);
    const double fs = std::stod(argv[3]);

    std::vector<int> rpeaks = EcgDetector::offlinedetectFromFile(input_file, detector_type, fs);

    std::cout << "QRS detected at index:";
    for (int index : rpeaks) {
        std::cout << " " << index;
    }
    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}