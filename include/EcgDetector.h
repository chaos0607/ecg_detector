#ifndef ECGDETECTOR_H
#define ECGDETECTOR_H

#include <vector>
#include <string>
#include <stdexcept>

class EcgDetector {
public:

    static std::vector<int> offlinedetect(const std::vector<double>& unfiltered_ecg, int detector_type, double fs);

    static std::vector<int> offlinedetectFromFile(const std::string& filename, int detector_type, double fs);
};

#endif // ECGDETECTOR_H