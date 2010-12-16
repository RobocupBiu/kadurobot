/* 
 * File:   Threshold.h
 * Author: dudi
 *
 * Created on November 29, 2010, 4:44 PM
 */
#include <cv.h>
#include <highgui.h>
#include <iostream>
#ifndef THRESHOLD_H
#define	THRESHOLD_H
using namespace std;
struct Gauss
{
    float mean[3];
    float var[3];
};

class Threshold {
public:
    Threshold();
    ~Threshold();
    void RGB_Prob_Threshold(IplImage* frame, Gauss prob, float k);
    void Gray_Prob_Threshold(IplImage* frame, Gauss prob, float k);
    void Prob_Threshold(IplImage* frame, Gauss prob, float k);
    Gauss Calculate_Prob(CvRect box, IplImage* frame);
private:
};

#endif	/* THRESHOLD_H */

