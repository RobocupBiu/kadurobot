/* 
 * File:   Video_Feed.h
 * Author: Dudi Likvornik & Yaron Naor
 *
 * Created on November 22, 2010, 9:27 PM
 */

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Camera.h"
#include "Camera_Observer.h"
using namespace std;
//using namespace cv;

#ifndef VIDEO_FEED_H
#define	VIDEO_FEED_H

class Video_Feed : public Camera_Observer
{
public:
    Video_Feed(Camera* camera);
    virtual ~Video_Feed();
    void Update_frame(IplImage* new_frame);
private:
    IplImage* frame;
    Camera* camera;
};

#endif	/* VIDEO_FEED_H */

