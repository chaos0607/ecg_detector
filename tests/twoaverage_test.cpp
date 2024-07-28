#include "detectors/TwoAverage.h"
#include <iostream>

int main() {

    std::vector<double> unfiltered_ecg;

	FILE *finput = fopen("../example_data/ECG.tsv","rt");
	for(;;) 
	{
		float a1,a2,a3,a4,a5,a6;
		if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
        unfiltered_ecg.push_back(a1);
	}
    const float fs = 250;

    TwoAverageDetector detector(fs);
    std::vector<int> qrs = detector.detect(unfiltered_ecg);

    for (int index : qrs) {
        std::cout << "QRS detected at index: " << index << std::endl;
    }

    return 0;
}