/* 
 * File:   Camera.h
 * Author: Dudi Likvornik & Yaron Naor
 *
 * Created on November 22, 2010, 9:22 PM
 */
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Camera_Observer.h"
using namespace std;
//using namespace cv;
#ifndef CAMERA_H
#define	CAMERA_H

class Camera {
public:
    static Camera* create_camera();     //will be use to create a single instance of Camera*
    void Get_frame();                   //a function which will get a new frame from the camera
    void Observer_Attach(Camera_Observer* observer);    //will add a new observer to the list
    void Observer_Detach(Camera_Observer* observer);    //wiil detach a specific observer from the list
    ~Camera();                                          //destructor
private:
    static Camera* camera_pointer;              //the camera
    static CvCapture* capture;                 //the actuall camera reasource
    IplImage* frame;                            //the frame
    Camera_Observer* observers_list;            //the observers list
    Camera(CvCapture** new_capture);            //a private constructor


};

#endif	/* CAMERA_H */

