#ifndef CALLBACK_H
#define CALLBACK_H

/**
 * @brief Heartrate detector callback interface
 * This interface establishes the communication between
 * the main program and the detector. The main program
 * needs to create an instance of this interface
 * and register it with the detecter class.
 */
struct HRCallback {
    /**
     * @brief Callback which is called whenever the heartrate has been determined
     * 
     * @param hr The heartrate in beats per minute.
     */
    virtual void hasHR(float hr) = 0;
};

#endif // CALLBACK_H