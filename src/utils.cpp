// utils.cpp
#include "utils.h"
#include <fstream>
#include <iostream>

void saveReaksToFile(const std::vector<u_int>& rpeaks, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "fail to openfile: " << filename << std::endl;
        return;
    }

    for (u_int value : rpeaks) {
        outFile << value << " ";
    }
    outFile << std::endl;

    outFile.close();
}

void saveEcgDataToFile(const std::vector<double>& data, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "fail to open file: " << filename << std::endl;
        return;
    }

    for (const auto& value : data) {
        outFile << value << "\n";
    }

    outFile.close();
}

std::vector<double> loadEcgDataFromFile(const std::string& filename) {
    std::vector<double> data;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "fail to open file: " << filename << std::endl;
        return data;
    }

    double value;
    while (inFile >> value) {
        data.push_back(value);
    }

    inFile.close();
    return data;
}