#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "EcgDetector.h"
#include "detectors/engzee.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "PlotEcg.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <sample file> <sample frequency>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string input_file = argv[1];
    const double fs = std::stod(argv[2]);

    Fl_Window* window = new Fl_Window(800, 600, "Real-time ECG");
    ECGWidget* ecg = new ECGWidget(fs,input_file,10, 10, 780, 580);
    window->end();
    window->show(argc, argv);

    return Fl::run();
}