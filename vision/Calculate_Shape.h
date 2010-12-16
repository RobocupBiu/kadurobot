/* 
 * File:   Calculate_Shape.h
 * Author: Dudi & Yaron
 *
 * Created on November 29, 2010, 7:26 PM
 */
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;

struct Matrix
{
    float precent;
    int group ;
};

struct Line
{
    CvPoint point1;
    CvPoint point2;
    float slope;
    Line* next;
};

struct Raw_Borders
{
    bool is_rect;
    bool is_left;
    CvRect rect;
    Line left;
};

#ifndef CALCULATE_SHAPE_H
#define	CALCULATE_SHAPE_H

class Calculate_Shape {
public:
    Calculate_Shape();
    ~Calculate_Shape();
    float Calculate_Precentage(IplImage* frame, int x_cord, int y_cord, int width);
    void Draw_Box(IplImage* frame,CvRect rect);
    void Calculate_Contours(IplImage* frame, IplImage* binnary_frame);
    CvPoint Pixel_Pixel(IplImage* binnary_frame);
private:
    CvRect box;
    Line* line_list_ancor;
    Raw_Borders borders;

    CvPoint Calculate_Center(CvSeq* contour);
    void Detect_Field_Borders(IplImage* frame);
    void Delete_Line_List(Line* line_list_ancor);
    Raw_Borders Calculate_Raw_Borders(IplImage* frame);

};

#endif	/* CALCULATE_SHAPE_H */

