#ifndef TWOAVERAGEDETECTOR_H
#define TWOAVERAGEDETECTOR_H

#include <vector>
#include <string>
#

class TwoAverageDetector {
public:
    TwoAverageDetector(double fs);
    std::vector<int> detect(const std::vector<double>& unfiltered_ecg);

private:
    double fs;
};

#endif // TWOAVERAGEDETECTOR_H