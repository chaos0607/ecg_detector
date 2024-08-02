// PlotEcg.h
#ifndef PLOTECG_H
#define PLOTECG_H

#include <FL/Fl_Widget.H>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "onlineData.h"

class ECGWidget : public Fl_Widget {
    std::vector<double> xdata;
    std::vector<double> ydata;
    std::vector<float> heartrate;
    std::queue<DataPoint> data_queue;
    std::mutex mtx;
    std::condition_variable cv;
    OnlineData* data_generator;

public:
    ECGWidget(double fs, std::string path, int X, int Y, int W, int H, const char* L = 0);
    ~ECGWidget();

    void update_ydata();
    void draw() override;

    static void Timer_CB(void* userdata);
};

#endif // PLOTECG_H