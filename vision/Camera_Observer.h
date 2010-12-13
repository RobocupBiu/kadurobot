/* 
 * File:   Camera_Observer.h
 * Author: Dudi Likvornik & Yaron Naor
 *
 * Created on November 22, 2010, 9:24 PM
 */
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;
//using namespace cv;

#ifndef CAMERA_OBSERVER_H
#define	CAMERA_OBSERVER_H

class Camera_Observer
{
    public:
        Camera_Observer* next;  //Will point to the next element in the observer list.
        Camera_Observer* prev;  //Will point to the next element in the observer list.
        Camera_Observer();      //constructor
        virtual  ~Camera_Observer();    //destructor
        virtual void Update_frame(IplImage*);    //This function will update the frame in all the observers

};

#endif	/* CAMERA_OBSERVER_H */

