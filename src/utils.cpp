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