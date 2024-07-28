#include <vector>
#include <numeric>
#include <algorithm>


std::vector<double> MWA_cumulative(const std::vector<double>& input_array, size_t window_size) {
    std::vector<double> ret(input_array.size(), 0.0);
    std::partial_sum(input_array.begin(), input_array.end(), ret.begin());

    for (size_t i = ret.size(); i-- > window_size;) {
        ret[i] = ret[i] - ret[i - window_size];
    }

    for (size_t i = 1; i < window_size; ++i) {
        ret[i - 1] = ret[i - 1] / static_cast<double>(i);
    }
    for (size_t i = window_size - 1; i < ret.size(); ++i) {
        ret[i] = ret[i] / static_cast<double>(window_size);
    }

    return ret;
}

std::vector<double> MWA_original(const std::vector<double>& input_array, size_t window_size) {
    std::vector<double> mwa(input_array.size(), 0.0);
    double sum = 0.0;

    for (size_t i = 0; i < input_array.size(); ++i) {
        sum += input_array[i];
        if (i >= window_size) {
            sum -= input_array[i - window_size];
        }
        if (i < window_size - 1) {
            mwa[i] = sum / static_cast<double>(i + 1);
        } else {
            mwa[i] = sum / static_cast<double>(window_size);
        }
    }

    return mwa;
}

std::vector<double> MWA_convolve(const std::vector<double>& input_array, size_t window_size) {
    std::vector<double> padded_input(window_size - 1, 0.0);
    padded_input.insert(padded_input.end(), input_array.begin(), input_array.end());

    std::vector<double> ret(padded_input.size() - window_size + 1, 0.0);
    std::vector<double> window(window_size, 1.0);

    for (size_t i = 0; i < ret.size(); ++i) {
        ret[i] = std::inner_product(padded_input.begin() + i, padded_input.begin() + i + window_size, window.begin(), 0.0);
    }

    for (size_t i = 1; i < window_size; ++i) {
        ret[i - 1] = ret[i - 1] / static_cast<double>(i);
    }
    for (size_t i = window_size - 1; i < ret.size(); ++i) {
        ret[i] = ret[i] / static_cast<double>(window_size);
    }

    return ret;
}