// PlotEcg.cpp
#include "PlotEcg.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <thread>

ECGWidget::ECGWidget(double fs, std::string path,int X, int Y, int W, int H, const char* L)
    : Fl_Widget(X, Y, W, H, L) {
    xdata.reserve(750); // 3s of data
    ydata.reserve(750);
    heartrate.reserve(750);
    for (int i = 0; i < 750; ++i) {
        xdata.push_back(i);
        ydata.push_back(0);
        heartrate.push_back(0);
    }
    data_generator = new OnlineData(data_queue, mtx, cv,fs,path);
    std::thread data_thread(&OnlineData::generate_data, data_generator);
    data_thread.detach();
    Fl::add_timeout(0.033, Timer_CB, (void*)this); // 33ms
}

ECGWidget::~ECGWidget() {
    delete data_generator;
}

void ECGWidget::update_ydata() {
    std::vector<DataPoint> new_data_points;
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (!data_queue.empty()) {
            new_data_points.push_back(data_queue.front());
            data_queue.pop();
        }
    }
    for (const DataPoint& data : new_data_points) {
        ydata.erase(ydata.begin());
        ydata.push_back(data.ecg_data);
        heartrate.erase(heartrate.begin());
        heartrate.push_back(data.heart_rate);
    }
    redraw();
}

void ECGWidget::draw() {
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h(), FL_WHITE);
    fl_color(FL_BLUE);
    fl_begin_line();
    for (size_t i = 0; i < ydata.size(); ++i) {
        fl_vertex(x() + static_cast<double>(i) * w() / 750.0, y() + h() / 2 - ydata[i] * h() * 120);
    }
    fl_end_line();

    float last_heartrate = 0.0;
    for (size_t i = 0; i < ydata.size(); ++i) {
        if (heartrate[i] != 0) {
            fl_color(FL_RED);
            fl_circle(x() + static_cast<double>(i) * w() / 750.0, y() + h() / 2 - ydata[i] * h() * 120, 10);
            last_heartrate = heartrate[i];
        }
    }

    char heartrate_str[10];
    snprintf(heartrate_str, sizeof(heartrate_str), "%.1f", last_heartrate);

    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA, 16);
    std::string heartrate_text = "Heart Rate: " + std::string(heartrate_str);
    fl_draw(heartrate_text.c_str(), x() + 300, y() + h() - 100);

}

void ECGWidget::Timer_CB(void* userdata) {
    ECGWidget* widget = (ECGWidget*)userdata;
    widget->update_ydata();
    Fl::repeat_timeout(0.033, Timer_CB, userdata); // 33ms
}