/*
 * File:   Histogram.cpp
 * Author: dudi
 *
 * Created on November 26, 2010, 9:43 PM
 */

#include "Histogram.h"

Histogram::Histogram()
{
    cvNamedWindow("Histogram_layer1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Histogram_layer2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Histogram_layer3", CV_WINDOW_AUTOSIZE);
}

Histogram::~Histogram()
{
    cvDestroyWindow("Histogram_layer1");
    cvDestroyWindow("Histogram_layer2");
    cvDestroyWindow("Histogram_layer3");
}

void Histogram::Calc_Histogram(IplImage* frame)
{
   IplImage* duplicate_frame = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, frame->nChannels );
   IplImage* duplicate_gray_frame = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1 );
   IplImage* layer1 = cvCreateImage( cvGetSize(frame), frame->depth, 1 );
   IplImage* layer2 = cvCreateImage( cvGetSize(frame), frame->depth, 1 );
   IplImage* layer3 = cvCreateImage( cvGetSize(frame), frame->depth, 1 );
   IplImage *hist_img_layer1 = cvCreateImage(cvSize(600,240), frame->depth, 1);
   IplImage *hist_img_layer2 = cvCreateImage(cvSize(300,240), frame->depth, 1);
   IplImage *hist_img_layer3 = cvCreateImage(cvSize(300,240), frame->depth, 1);
   cvSet( layer1, cvScalarAll(255), 0 );
   cvSet( layer2, cvScalarAll(255), 0 );
   cvSet( layer3, cvScalarAll(255), 0 );

   CvHistogram *hist_layer1;
   CvHistogram *hist_layer2;
   CvHistogram *hist_layer3;

    int hist_size = 256;
    float range[]={0,256};
    float* ranges[] = { range };
    float max_value = 0.0;
    float max = 0.0;
    float w_scale = 0.0;

    /* Create a 1-D Arrays to hold the histograms */
    hist_layer1 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
    hist_layer2 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
    hist_layer3 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
    if(duplicate_frame->nChannels == 1)
    {
        cvCopyImage(frame,duplicate_gray_frame);
    }
    /* Set image to obtain RED as Channel of Interest (COI) */
    if(duplicate_frame->nChannels > 1)
    {
        cvCvtColor(frame,duplicate_frame,CV_BGR2HSV);
        cvSplit(duplicate_frame,layer1,layer2,layer3,NULL);
    }

    /* Calculate histogram of the Image and store it in the array */
    if(duplicate_frame->nChannels > 1)
    {
        cvCalcHist( &layer1, hist_layer1, 0, NULL );
        cvCalcHist( &layer2, hist_layer2, 0, NULL );
        cvCalcHist( &layer3, hist_layer3, 0, NULL );
    }
    else
        cvCalcHist( &duplicate_gray_frame, hist_layer1, 0, NULL );
    /* Find the minimum and maximum values of the histograms */
    cvGetMinMaxHistValue( hist_layer1, 0, &max_value, 0, 0 );
    if(duplicate_frame->nChannels > 1)
    {
        cvGetMinMaxHistValue( hist_layer2, 0, &max, 0, 0 );
        max_value = (max > max_value) ? max : max_value;
        cvGetMinMaxHistValue( hist_layer3, 0, &max, 0, 0 );
        max_value = (max > max_value) ? max : max_value;
    }
    // The variable max_value has the maximum of the three histograms
    /* Using the maximum value, Scale/Squeeze the histogram (to fit the image) */
    cvScale( hist_layer1->bins, hist_layer1->bins, ((float)hist_img_layer1->height)/max_value, 0 );
    if(duplicate_frame->nChannels > 1)
    {
        cvScale( hist_layer2->bins, hist_layer2->bins, ((float)hist_img_layer2->height)/max_value, 0 );
        cvScale( hist_layer3->bins, hist_layer3->bins, ((float)hist_img_layer3->height)/max_value, 0 );
    }
    /* Scale/Squeeze the histogram range to image width */
    w_scale = ((float)hist_img_layer1->width)/hist_size;
    /* Plot the Histograms */
    for( int i = 0; i < hist_size; i++ )
    {
      cvRectangle( hist_img_layer1, cvPoint((int)i*w_scale , hist_img_layer1->height),
        cvPoint((int)(i+1)*w_scale, hist_img_layer1->height - cvRound(cvGetReal1D(hist_layer1->bins,i))),
        CV_RGB(255,255,255), -1, 8, 0 );
      if(duplicate_frame->nChannels > 1)
      {
        cvRectangle( hist_img_layer2, cvPoint((int)i*w_scale , hist_img_layer2->height),
        cvPoint((int)(i+1)*w_scale, hist_img_layer2->height - cvRound(cvGetReal1D(hist_layer2->bins,i))),
        CV_RGB(255,255,255), -1, 8, 0 );
        cvRectangle( hist_img_layer3, cvPoint((int)i*w_scale , hist_img_layer3->height),
        cvPoint((int)(i+1)*w_scale, hist_img_layer3->height - cvRound(cvGetReal1D(hist_layer3->bins,i))),
        CV_RGB(255,255,255), -1, 8, 0 );
      }
    }
    /* create a window to show the histogram of the image */
    cvShowImage( "Histogram_layer1", hist_img_layer1);
    if(duplicate_frame->nChannels > 1)
    {
        cvShowImage( "Histogram_layer2", hist_img_layer2);
        cvShowImage( "Histogram_layer3", hist_img_layer3);
    }
    cvReleaseImage( &hist_img_layer1 );
    cvReleaseImage( &hist_img_layer2 );
    cvReleaseImage( &hist_img_layer3 );
    cvReleaseImage( &duplicate_frame );
    cvReleaseImage( &duplicate_gray_frame );
    cvReleaseImage( &layer1 );
    cvReleaseImage( &layer2 );
    cvReleaseImage( &layer3 );
    cvReleaseHist(&hist_layer1);
    cvReleaseHist(&hist_layer2);
    cvReleaseHist(&hist_layer3);
}