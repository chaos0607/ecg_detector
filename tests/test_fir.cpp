// Floating point example
//

// This is the only include you need
#include "detectors/engzee.h"
#include "math.h"

#include <stdio.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Iir.h" 

void assertEqual(float a, float b) {
	if (fabs(a-b) < 1E-10) return;
	printf("BUG: %f != %f\n",a,b);
	exit(1);
}

int main() {

	const float fs = 250;
    const float mains = 50;
	Iir::Butterworth::BandStop<2> iirnotch;
	iirnotch.setup(fs,mains,2);



	std::vector<double> unfiltered_ecg;
    std::string filename = std::string(PROJECT_ROOT) + "/example_data/ECG.tsv";

    std::ifstream finput(filename);
	std::ofstream outfile("filtered_ecg_full_c++.txt");

    if (!finput.is_open()) {
        std::cerr << "Error: fail to open file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    double cs_V2_V1, einthoven_II, einthoven_III, acc_x, acc_y, acc_z;
    while (finput >> cs_V2_V1 >> einthoven_II >> einthoven_III >> acc_x >> acc_y >> acc_z) {
        unfiltered_ecg.push_back(cs_V2_V1);
		std::cout << cs_V2_V1 << std::endl;
		double a = iirnotch.filter(cs_V2_V1);
		std::cout << a << std::endl;
		outfile << a << std::endl;
    }
}

