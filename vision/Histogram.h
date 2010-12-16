/* 
 * File:   Histogram.h
 * Author: dudi
 *
 * Created on November 26, 2010, 10:39 PM
 */
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;
//using namespace cv;
#ifndef HISTOGRAM_H
#define	HISTOGRAM_H

class Histogram
{
    public:
        Histogram();
        ~Histogram();
        void Calc_Histogram(IplImage* frame);           //get the histogram
};

#endif	/* HISTOGRAM_H */

