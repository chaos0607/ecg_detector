// PlotEcg.h
#ifndef PLOTECG_H
#define PLOTECG_H

#include <FL/Fl_Widget.H>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "onlineData.h"

/**
 * @class ECGWidget
 * @brief A widget for displaying ECG data and heart rate using the FLTK library.
 * 
 * This class is responsible for rendering ECG data and heart rate on a graphical interface.
 */
class ECGWidget : public Fl_Widget {
    std::vector<double> xdata; ///< X-axis data for plotting ECG.
    std::vector<double> ydata; ///< Y-axis data for plotting ECG.
    std::vector<float> heartrate; ///< Vector to store heart rate values.
    std::queue<DataPoint> data_queue;
    std::mutex mtx;
    std::condition_variable cv;
    OnlineData* data_generator;

public:
    /**
     * @brief Construct a new ECGWidget object.
     * 
     * Initializes the ECG data plotting widget, sets up the data generator, and starts the update timer.
     * 
     * @param fs The sampling frequency of the ECG data.
     * @param path Path to the input file containing ECG data.
     * @param X The X coordinate of the widget.
     * @param Y The Y coordinate of the widget.
     * @param W The width of the widget.
     * @param H The height of the widget.
     * @param L The label of the widget (optional).
     */
    ECGWidget(double fs, std::string path, int X, int Y, int W, int H, const char* L = 0);

    /**
     * @brief Destroy the ECGWidget object.
     * 
     * Cleans up resources used by the widget, including the data generator.
     */
    ~ECGWidget();

    /**
     * @brief Update the Y-axis data for ECG plotting.
     * 
     * This method retrieves new data points from the queue and updates the Y-axis data
     * vector for ECG plotting.
     */
    void update_ydata();

    /**
     * @brief Override the draw method to render ECG data and heart rate.
     * 
     * This method is called by the FLTK library to draw the widget, rendering the
     * ECG waveform and heart rate on the screen.
     */
    void draw() override;

    /**
     * @brief Timer callback function for updating the ECG plot.
     * 
     * This static method is called periodically by the FLTK timer to update the
     * ECG data plot and trigger a redraw of the widget.
     * 
     * @param userdata Pointer to the ECGWidget instance.
     */
    static void Timer_CB(void* userdata);
};

#endif // PLOTECG_H
