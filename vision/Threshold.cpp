/* 
 * File:   Threshold.cpp
 * Author: dudi
 * 
 * Created on November 29, 2010, 4:44 PM
 */

#include "Threshold.h"

Threshold::Threshold()
{
    cvNamedWindow("Color_RGB_Threshold_Detection_Result", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Color_Gray_Threshold_Detection_Result", CV_WINDOW_AUTOSIZE);
}

Threshold::~Threshold() {
}

void Threshold::RGB_Prob_Threshold(IplImage* frame, Gauss prob, float k)
{
    IplImage* threshold_frame = cvCreateImage( cvSize(frame->width,frame->height), frame->depth, frame->nChannels );
    cvCopyImage(frame,threshold_frame);
    for( int y=0; y<threshold_frame->height; y++ )
    {
        uchar* ptr = (uchar*) (threshold_frame->imageData + y * threshold_frame->widthStep);
        for( int x=0; x<threshold_frame->width; x++ )
        {
            if(ptr[threshold_frame->nChannels*x+0] <= prob.mean[0]+k*prob.var[0] &&
               ptr[threshold_frame->nChannels*x+0] >= prob.mean[0]-k*prob.var[0])
            {
               if(ptr[threshold_frame->nChannels*x+1] <= prob.mean[1]+k*prob.var[1] &&
                  ptr[threshold_frame->nChannels*x+1] >= prob.mean[1]-k*prob.var[1])
               {
                   if(ptr[threshold_frame->nChannels*x+2] <= prob.mean[2]+k*prob.var[2] &&
                      ptr[threshold_frame->nChannels*x+2] >= prob.mean[2]-k*prob.var[2])
                   {
                       ptr[x*threshold_frame->nChannels +0] = prob.mean[0];
                       ptr[x*threshold_frame->nChannels +1] = prob.mean[1];
                       ptr[x*threshold_frame->nChannels +2] = prob.mean[2];
                   }
                   else
                   {
                        ptr[x*threshold_frame->nChannels +0] = 0;
                        ptr[x*threshold_frame->nChannels +1] = 0;
                        ptr[x*threshold_frame->nChannels +2] = 0;
                   }
               }
               else
               {
                   ptr[x*threshold_frame->nChannels +0] = 0;
                   ptr[x*threshold_frame->nChannels +1] = 0;
                   ptr[x*threshold_frame->nChannels +2] = 0;
               }
            }
            else
            {
               ptr[x*threshold_frame->nChannels +0] = 0;
               ptr[x*threshold_frame->nChannels +1] = 0;
               ptr[x*threshold_frame->nChannels +2] = 0;
            }
        }
    }
    cvShowImage("Color_RGB_Threshold_Detection_Result", threshold_frame);
    cvReleaseImage(&threshold_frame);
}

void Threshold::Gray_Prob_Threshold(IplImage* frame, Gauss prob, float k)
{
    IplImage* threshold_frame = cvCreateImage( cvSize(frame->width,frame->height), frame->depth, 1 );
    cvCopyImage(frame,threshold_frame);
    for( int y=0; y<threshold_frame->height; y++ )
    {
        uchar* ptr = (uchar*) (threshold_frame->imageData + y * threshold_frame->widthStep);
        for( int x=0; x<threshold_frame->width; x++ )
        {
            if(ptr[threshold_frame->nChannels*x+0] <= prob.mean[0]+k*prob.var[0] &&
               ptr[threshold_frame->nChannels*x+0] >= prob.mean[0]-k*prob.var[0])
                ptr[x*threshold_frame->nChannels +0] = prob.mean[0];
            else
                ptr[x*threshold_frame->nChannels +0] = 0;
        }
    }
    cvShowImage("Color_Gray_Threshold_Detection_Result", threshold_frame);
    cvReleaseImage(&threshold_frame);
}

/*Description: The function will detect a specific color.
 * Function: The function will detect a specific value by normalizing samples from a specific
 *           rectangle.
 * Parameters: box - a specific retangle which will be our sampaling ground,
 *             the frame we will be sampaling from.
 * Return Value: the mean and variance of the sampales.*/
Gauss Threshold::Calculate_Prob(CvRect box, IplImage* frame)
{
    Gauss result;
    for(int index = 0; index < 3; index++)
    {
        result.mean[index] = 0;
        result.var[index] = 0;
    }
    for( int y=box.y + 1; y<box.height+box.y; y++ )
    {
        uchar* ptr = (uchar*) (frame->imageData + y * frame->widthStep);
        for( int x=box.x + 1; x<box.x+box.width; x++ )
        {
            result.mean[0] += (float)ptr[frame->nChannels*x+0];
            result.mean[1] += (float)ptr[frame->nChannels*x+1];
            result.mean[2] += (float)ptr[frame->nChannels*x+2];
        }
    }
    result.mean[0] = result.mean[0] / (box.height*box.width);
    result.mean[1] = result.mean[1] / (box.height*box.width);
    result.mean[2] = result.mean[2] / (box.height*box.width);
    cout<<"blue mean:"<<result.mean[0]<<
            " green mean:"<<result.mean[1]<<" red mean:"<<result.mean[2]<<endl;
    for( int y=box.y + 1; y<box.height+box.y - 1; y++ )
    {
        uchar* ptr = (uchar*) (frame->imageData + y * frame->widthStep);
        for( int x=box.x + 1; x<box.x+box.width - 1; x++ )
        {
            result.var[0] += pow(ptr[frame->nChannels*x+0] - result.mean[0],2);
            result.var[1] += pow(ptr[frame->nChannels*x+1] - result.mean[1],2);
            result.var[2] += pow(ptr[frame->nChannels*x+2] - result.mean[2],2);
        }
    }
    result.var[0] = result.var[0] / (box.height*box.width - 1);
    result.var[1] = result.var[1] / (box.height*box.width - 1);
    result.var[2] = result.var[2] / (box.height*box.width - 1);
    result.var[0] = sqrt(result.var[0]);
    result.var[1] = sqrt(result.var[1]);
    result.var[2] = sqrt(result.var[2]);
    cout<<"blue dev:"<<result.var[0]<<" green dev:"<<result.var[1]<<" red dev:"<<result.var[2]<<endl;
    return result;
}