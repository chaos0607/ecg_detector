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

int main() {
    std::string directoryPath = "/home/cc/work/project/code/dataset/experiment_data"; // 指定你的目录路径
    processECGFilesInDirectory(directoryPath,0,250);
    processECGFilesInDirectory(directoryPath,1,250);
    return 0;
}