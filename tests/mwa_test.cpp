#include <iostream>
#include <vector>
#include <numeric>
#include "algorithms/mwa.h"
#include <fstream>
#include <iomanip> 
#include <algorithm>

int main() {

    


    std::vector<double> input_array;
    std::string filename = std::string(PROJECT_ROOT) + "/example_data/ECG.tsv";

    std::ifstream finput(filename);
    if (!finput.is_open()) {
        std::cerr << "Error: fail to open file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }
    double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

    while (finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z) {
        input_array.push_back(cs_V2_V1);
    }

    size_t window_size = static_cast<size_t>(0.12*250);

    std::vector<double> abs_input_array(input_array.size());
    std::transform(input_array.begin(), input_array.end(), abs_input_array.begin(), [](double val) {
        return std::abs(val);
    });

    std::vector<double> result_cumulative = MWA_cumulative(abs_input_array, window_size);
    std::vector<double> result_original = MWA_original(abs_input_array, window_size);
    std::vector<double> result_convolve = MWA_convolve(abs_input_array, window_size);



    std::ofstream file_cumulative("result_cumulative.txt");
    file_cumulative << std::fixed << std::setprecision(6);
    for (const auto& value : result_cumulative) {
        file_cumulative << value << "\n";
    }
    file_cumulative.close();

    std::ofstream file_original("result_original.txt");
    file_original << std::fixed << std::setprecision(6);
    for (const auto& value : result_original) {
        file_original << value << "\n";
    }
    file_original.close();

    std::ofstream file_convolve("result_convolve.txt");
    file_convolve << std::fixed << std::setprecision(6);
    for (const auto& value : result_convolve) {
        file_convolve << value << "\n";
    }
    file_convolve.close();

    return 0;
}