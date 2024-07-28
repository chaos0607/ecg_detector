#ifndef FIR_H
#define FIR_H

#include <vector>
#include <deque>

class Fir {
public:

    std::vector<float> coefficients;
    std::deque<float> buffer;
    
    void init(const std::vector<float> _coefficients) {
        coefficients = _coefficients;
        buffer = std::deque<float>();
        buffer.resize(coefficients.size());
    }

    void init(const int length) {
        buffer = std::deque<float>();
        buffer.resize(length);
        coefficients = std::vector<float>();
        coefficients.resize(length);
    }

    inline void push(const float input) {
        buffer.push_front(input);
        buffer.pop_back();
    }

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

    inline float average() const {
        float a = 0.0;
        for(auto &v:buffer) {
            a += v;
        }
        return a / (float)(buffer.size());
    }
};

#endif // FIR_H