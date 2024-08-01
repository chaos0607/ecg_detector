#include <iostream>
#include <vector>
#include <fstream>
#include "EcgDetector.h"
#include <filesystem>

namespace fs = std::filesystem;

void processECGFilesInDirectory(const std::string& directoryPath, int detectorType, double fs) {
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().filename() == "ECG.tsv") {
            
            std::string inputFilePath = entry.path().string();
            std::string outputFilePath = entry.path().parent_path().string() + "/rpeaks.tsv";
            if (detectorType == 0) {
                outputFilePath = entry.path().parent_path().string() + "/rpeaks_twoaverage.tsv";
            } else if (detectorType == 1) {
                outputFilePath = entry.path().parent_path().string() + "/rpeaks_wqrs.tsv";
            } else if (detectorType == 2) {
                outputFilePath = entry.path().parent_path().string() + "/rpeaks_engzee.tsv";
            }


            std::cout << "Processing file: " << inputFilePath << std::endl;
            std::vector<int> rpeaks = EcgDetector::offlinedetectFromFile(inputFilePath, detectorType,fs);

            std::ofstream outputFile(outputFilePath);
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open file: " << outputFilePath << std::endl;
                continue;
            }

            for (const int& peak : rpeaks) {
                outputFile << peak << std::endl;
            }

            outputFile.close();

        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <sample database path> <sample frequency>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string input_file = argv[1];
    const double fs = std::stod(argv[2]);

    std::string directoryPath = input_file;
    processECGFilesInDirectory(directoryPath,0,fs); // 0 for TwoAverage
    processECGFilesInDirectory(directoryPath,1,fs); // 1 for Wqrs
    processECGFilesInDirectory(directoryPath,2,fs); // 2 for engzee
    return 0;
}