#ifndef MWA_H
#define MWA_H

#include <vector>
#include <iostream>

/**
 * @brief Computes the cumulative sum for moving window average.
 *
 * This function calculates the moving window average using the cumulative sum method.
 * It is useful for efficiently computing moving averages over large datasets.
 *
 * @param input_array The input array of data points.
 * @param window_size The size of the moving window.
 * @return A vector containing the moving window averages.
 */
std::vector<double> MWA_cumulative(const std::vector<double>& input_array, size_t window_size);

/**
 * @brief Computes the moving window average using the original method.
 *
 * This function implements the basic moving window average calculation by directly
 * summing elements within the window and dividing by the window size.
 *
 * @param input_array The input array of data points.
 * @param window_size The size of the moving window.
 * @return A vector containing the moving window averages.
 */
std::vector<double> MWA_original(const std::vector<double>& input_array, size_t window_size);

/**
 * @brief Computes the moving window average using 1D convolution.
 *
 * This function calculates the moving window average by convolving the input array
 * with a kernel of equal weights. This method is typically faster for large datasets.
 *
 * @param input_array The input array of data points.
 * @param window_size The size of the moving window.
 * @return A vector containing the moving window averages.
 */
std::vector<double> MWA_convolve(const std::vector<double>& input_array, size_t window_size);

#endif // MWA_H
