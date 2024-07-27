#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iostream>

// cumulative sum
std::vector<double> MWA_cumulative(const std::vector<double>& input_array, int window_size);

// Original Function 
std::vector<double> MWA_original(const std::vector<double>& input_array, int window_size);

// Fast moving window average implemented with 1D convolution 
std::vector<double> MWA_convolve(const std::vector<double>& input_array, int window_size);

#endif // COMMON_H