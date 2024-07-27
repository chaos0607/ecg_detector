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

int test1 (int,char**) {
	std::vector<float> c({1,3,2,-5,8,-8,7,9});
	Fir fir;
	fir.init(c);
	float acc = 0;
	for(int i = 0;i < 16;i++) {
		float v = 0;
		if ((i == 1) || (i > 10)) {
			v = 1;
		}
		// let's filter a delta pulse at time step 1 and
		// a unit step at time step 10
		v = fir.filter(v);
		// now let's check if the filter has done the right ops
		// the delta pulse should reproduce the impulse respnse
		if ( (i < 9) && (i > 0) ) {
			assertEqual(c[i-1],v);
		}
		// and the step response should accumulate it
		if (i > 10) {
			acc = acc + c[i - 11];
			assertEqual(v,acc);
		}
	}
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

