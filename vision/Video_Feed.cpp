/* 
 * File:   Video_Feed.cpp
 * Author: Dudi Likvornik & Yaron Naor
 * 
 * Created on November 21, 2010, 3:00 PM
 */
#include "Video_Feed.h"
Video_Feed::Video_Feed(Camera* new_camera)
{
    camera = new_camera;
    camera->Observer_Attach(this);
    cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );
}

Video_Feed::~Video_Feed()
{
    camera->Observer_Detach(this);
    cvDestroyWindow("Video");
}

void Video_Feed::Update_frame(IplImage* new_frame)
{
        frame = new_frame;
        cvShowImage( "Video", frame );
}