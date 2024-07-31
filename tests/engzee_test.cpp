#include "detectors/engzee.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Iir.h"
#include "utils.h"
#include "callback.h" 

struct MyCallback : HRCallback {
	FILE* f;
	MyCallback(FILE* hrFile) {
		f = hrFile;
	}
	virtual void hasHR(float hr) {
		printf("HR = %f\n",hr);
		fprintf(f,"%f\n",hr);
	}
};

int main (int,char**)
{
    std::vector<double> unfiltered_ecg;
	std::string filename = std::string(PROJECT_ROOT) + "/example_data/ECG.tsv";


	std::ifstream finput(filename);
    if (!finput.is_open()) {
        std::cerr << "Error: fail to open file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }
    double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;

    while (finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z) {
        unfiltered_ecg.push_back(cs_V2_V1);
    }
    const double fs = 250;

    EngzeeDetector detector(fs);
    std::vector<int> qrs = detector.OfflineDetect(unfiltered_ecg);

    std::cout << "QRS detected at index:";
    for (int index : qrs) {
        std::cout << " " << index;
    }
    std::cout << std::endl;

    return 0;
}
