#ifndef MWA_H
#define MWA_H

#include <vector>
#include <iostream>

// cumulative sum
std::vector<double> MWA_cumulative(const std::vector<double>& input_array, size_t window_size);

// Original Function 
std::vector<double> MWA_original(const std::vector<double>& input_array, size_t window_size);

// Fast moving window average implemented with 1D convolution 
std::vector<double> MWA_convolve(const std::vector<double>& input_array, size_t window_size);

#endif // MWA_H