#include "detectors/engzee.h"

#include <stdio.h>
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

	FILE *finput = fopen("../example_data/ECG.tsv","rt");
	for(;;) 
	{
		float a1,a2,a3,a4,a5,a6;
		if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
        unfiltered_ecg.push_back(a1);
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
