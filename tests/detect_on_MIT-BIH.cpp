#include <iostream>
#include <vector>
#include <fstream>
#include "EcgDetector.h"
#include <filesystem>

namespace fs = std::filesystem;
void processECGFilesInDirectory(const std::string& directoryPath, int detectorType, double fs) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".tsv" && entry.path().filename().string().rfind("d1_", 0) == 0) {
            
            std::string inputFilePath = entry.path().string();
            std::string filename = entry.path().stem().string();
            std::string outputFilePath;

            if (detectorType == 0) {
                outputFilePath = entry.path().parent_path().string() + "/rpeak_twoaverage_" + filename + ".tsv";
            } else if (detectorType == 1) {
                outputFilePath = entry.path().parent_path().string() + "/rpeak_wqrs_" + filename + ".tsv";
            } else if (detectorType == 2) {
                outputFilePath = entry.path().parent_path().string() + "/rpeak_engzee_" + filename + ".tsv";
            }

            std::cout << "Processing file: " << inputFilePath << std::endl;
            std::vector<int> rpeaks = EcgDetector::offlinedetectFromFile(inputFilePath, detectorType, fs);

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
    processECGFilesInDirectory(directoryPath,0,fs);
    processECGFilesInDirectory(directoryPath,1,fs);
    processECGFilesInDirectory(directoryPath,2,fs);
    return 0;
}