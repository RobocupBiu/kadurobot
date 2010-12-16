/* 
 * File:   Color_Detection.h
 * Author: Dudi Likvornik & Yaron Naor
 *
 * Created on November 22, 2010, 9:25 PM
 */

#include "Camera_Observer.h"
#include "Camera.h"
#include "Histogram.h"
#include "Threshold.h"
#include "Calculate_Shape.h"
using namespace std;
//using namespace cv;

#ifndef COLOR_DETECTION_H
#define	COLOR_DETECTION_H

class Color_Detection : public Camera_Observer
{
public:
    Color_Detection(Camera* camera, Histogram* new_hist,Threshold* new_threshold);
    virtual ~Color_Detection();
    void Update_frame(IplImage* new_frame);

private:
    Camera* camera;         //the camera
    IplImage* frame;        //the frame its self
    Histogram* hist;        //we want to use histogram
    Threshold* threshold;   //will be use to threshold a frame
    bool drawing_box;       //will tell us if to draw or not draw a box
    CvRect box;             //the box
    Gauss result;           //the result of the statistic calculation

    void Draw_box(IplImage* frame, CvRect rect);                        //this function will draw a box
    void Mouse_event(int event, int x, int y, int flags, void* param);                  //a callback for the mouse event
    void Smooth(IplImage** frame);                                                      //the function will do a low pass filtering
    void HSV_Segmentation(IplImage* frame);                                             //the function will do HSV segmentation
    static void Mouse_event_Wrapper(int event, int x, int y, int flags, void* param);   //a wrapper for the mouse event
};

#endif	/* COLOR_DETECTION_H */

