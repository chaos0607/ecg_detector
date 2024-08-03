#ifndef FIR_H
#define FIR_H

#include <vector>
#include <deque>

/**
 * @class Fir
 * @brief A Finite Impulse Response (FIR) filter class.
 *
 * This class implements a simple FIR filter using a vector of coefficients and a buffer for storing input samples.
 */
class Fir {
public:

    /**
     * @brief Coefficients of the FIR filter.
     */
    std::vector<float> coefficients;

    /**
     * @brief Buffer to store input samples.
     */
    std::deque<float> buffer;

    /**
     * @brief Initializes the FIR filter with a set of coefficients.
     * 
     * @param _coefficients A vector of filter coefficients.
     */
    void init(const std::vector<float> _coefficients) {
        coefficients = _coefficients;
        buffer = std::deque<float>();
        buffer.resize(coefficients.size());
    }

    /**
     * @brief Initializes the FIR filter with a specified length.
     * 
     * @param length The number of taps (coefficients) for the filter.
     */
    void init(const int length) {
        buffer = std::deque<float>();
        buffer.resize(length);
        coefficients = std::vector<float>();
        coefficients.resize(length);
    }

    /**
     * @brief Pushes a new input sample into the buffer.
     * 
     * @param input The new input sample to be added.
     */
    inline void push(const float input) {
        buffer.push_front(input);
        buffer.pop_back();
    }

    /**
     * @brief Filters the input sample and returns the output.
     * 
     * @param input The new input sample to be filtered.
     * @return The filtered output sample.
     */
    inline float filter(const float input) {
        push(input);
        float output = 0.0f;
        auto itcoeff = coefficients.begin();
        auto itbuffer = buffer.begin();
        for(unsigned i = 0; i < buffer.size(); i++) {
            output += *(itcoeff++) * *(itbuffer++);
        }
        return output;
    }

    /**
     * @brief Finds the maximum value in the buffer over a specified number of timesteps.
     * 
     * @param timesteps The number of recent samples to consider. If negative, considers the entire buffer.
     * @return The maximum value found in the specified range.
     */
    inline float max(const int timesteps = -1) const {
        int endpoint;
        if ( (timesteps < 0) || (timesteps > (int)buffer.size() ) ) {
            endpoint = (int)buffer.size();
        } else {
            endpoint = timesteps;
        }
        float max = 0;
        for (int i = 0; i < endpoint; i++) {
            if (buffer[i] > max) {
                max = buffer[i];
            }
        }
        return max;
    }

    /**
     * @brief Calculates the average of the values in the buffer.
     * 
     * @return The average value of the buffer.
     */
    inline float average() const {
        float a = 0.0;
        for(auto &v:buffer) {
            a += v;
        }
        return a / (float)(buffer.size());
    }
};

#endif // FIR_H
