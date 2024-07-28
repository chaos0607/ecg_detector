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


	FILE *finput = fopen("../example_data/ECG.tsv","rt");

	std::ofstream outfile("filtered_ecg_full_c++.txt");

	if (!outfile.is_open()) {
		std::cerr << "fail to open outfile" << std::endl;
		return 1;
	}

	for(;;) 
	{
		float a1,a2,a3,a4,a5,a6;
		if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
		std::cout << a1 << std::endl;
		const float a = iirnotch.filter(a1);
		std::cout << a << std::endl;
		outfile << a << std::endl;
	}
}

