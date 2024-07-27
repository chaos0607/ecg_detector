#include <iostream>
#include <vector>
#include <numeric>
#include "detectors/common.h"
#include <fstream>
#include <iomanip> 

int main() {

    

    std::vector<double> input_array;

	FILE *finput = fopen("../example_data/ECG.tsv","rt");
	for(;;) 
	{
		float a1,a2,a3,a4,a5,a6;
		if (fscanf(finput,"%f %f %f %f %f %f\n",&a1,&a2,&a3,&a4,&a5,&a6)<1) break;
        input_array.push_back(a1);
	}


    int window_size = 0.12*250;

    std::vector<double> result_cumulative = MWA_cumulative(input_array, window_size);
    std::vector<double> result_original = MWA_original(input_array, window_size);
    std::vector<double> result_convolve = MWA_convolve(input_array, window_size);



    std::ofstream file_cumulative("result_cumulative.txt");
    file_cumulative << std::fixed << std::setprecision(6);
    for (const auto& value : result_cumulative) {
        file_cumulative << value << "\n";
    }
    file_cumulative.close();

    // 保存 result_original 到文件
    std::ofstream file_original("result_original.txt");
    file_original << std::fixed << std::setprecision(6);
    for (const auto& value : result_original) {
        file_original << value << "\n";
    }
    file_original.close();

    // 保存 result_convolve 到文件
    std::ofstream file_convolve("result_convolve.txt");
    file_convolve << std::fixed << std::setprecision(6);
    for (const auto& value : result_convolve) {
        file_convolve << value << "\n";
    }
    file_convolve.close();

    return 0;
}