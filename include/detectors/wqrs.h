#ifndef WQRS_H
#define WQRS_H

#include <vector>

class wqrsDetector {
public:

    wqrsDetector(double fs);

    std::vector<double> lengthTransform(const std::vector<double>& x, int w, double fs);

    std::vector<int> threshold(const std::vector<double>& x, double fs);

    std::vector<int> detect(const std::vector<double>& unfiltered_ecg);

private:
    double fs;
};

#endif // WQRS_H