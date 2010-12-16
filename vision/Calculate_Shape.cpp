/* 
 * File:   Calculate_Shape.cpp
 * Author: dudi
 * 
 * Created on November 29, 2010, 7:26 PM
 */

#include "Calculate_Shape.h"

Calculate_Shape::Calculate_Shape()
{
}


Calculate_Shape::~Calculate_Shape()
{
}

void Calculate_Shape::Delete_Line_List(Line* line_list_ancor)
{
    Line* line_pointer1;
    Line* line_pointer2;
    line_pointer1 = line_list_ancor;
    while(line_pointer1)
    {
        line_pointer2 = line_pointer1->next;
        delete line_pointer1;
        line_pointer1 = line_pointer2;
    }
}

void Calculate_Shape::Draw_Box(IplImage* frame, CvRect rect)
{
    if((rect.y+rect.height <=  frame->height) && (rect.x+rect.width <= frame->width)
         && (rect.y >=  0) && (rect.x >= 0))
    {
        cvRectangle(
                frame,
                cvPoint(rect.x,rect.y),
                cvPoint(rect.x + rect.width, rect.y + rect.height),
                cvScalar(255,0,0),
                2);
    }
}

float Calculate_Shape::Calculate_Precentage(IplImage* frame,int x_cord, int y_cord, int width)
{
    int count = 0;
    if((y_cord+width <  frame->height) && (x_cord+width < frame->width))
    {
        if((y_cord - width>=0) && (x_cord-width >=0))
        {
            for( int y=y_cord - width; y<=y_cord+width; y++ )
            {
                uchar* ptr = (uchar*) (frame->imageData + y * frame->widthStep);
                for( int x=x_cord - width; x<=x_cord+width; x++ )
                {
                    if(ptr[x*frame->nChannels + 0] == 0 && ptr[x*frame->nChannels + 1] == 255 && ptr[x*frame->nChannels + 2] == 0)
                    {
                        count++;
                    }
                }
            }
        }
        return ((count/((2*width+1)*(2*width+1)))*100);
    }
    else
        return 0;
}

void Calculate_Shape::Calculate_Contours(IplImage* frame, IplImage* binnary_frame)
{
    CvSeq* contours = NULL;
    Line* line_pointer;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvPoint middle_point;
    int y_cord;
    bool is_ok;

    is_ok = false;
    middle_point.x = 0;
    middle_point.y = 0;
    
    CvRect rect;

    Detect_Field_Borders(frame);
    Calculate_Raw_Borders(frame);
  
    for( int y=0; y<binnary_frame->height; y++ )
    {
        uchar* ptr = (uchar*) (binnary_frame->imageData + y * binnary_frame->widthStep);
        for( int x=0; x<binnary_frame->width; x++ )
        {
            if(borders.is_rect)
            {
                if((ptr[x*binnary_frame->nChannels + 0] == 255) &&
               (borders.rect.x >= x) || (borders.rect.y >= y) || (borders.rect.x+borders.rect.width <= x))
                {
                    ptr[x*binnary_frame->nChannels + 0] = 0;
                }
                else
                    is_ok = true;
            }
            if(borders.is_left && (is_ok == false))
            {
                y_cord = borders.left.point1.y*-1 -1*borders.left.slope*(x - borders.left.point1.x);
                y_cord = y_cord*-1;
                if((ptr[x*binnary_frame->nChannels + 0] == 255) &&
               (borders.left.point1.x >= x) || (borders.left.point2.x <= x) || (y_cord >= y))
                {
                    ptr[x*binnary_frame->nChannels + 0] = 0;
                }
            }
            is_ok = false;
        }
    }

    cvFindContours(binnary_frame,storage,&contours);

    while( contours )
    {
        middle_point = Calculate_Center(contours);
        if(middle_point.x != -1 && middle_point.y != -1)
        {
            rect.height = 5;
            rect.width = 5;
            rect.x = middle_point.x;
            rect.y = middle_point.y;
            Draw_Box(frame,rect);
            cvDrawContours(frame,contours,cvScalarAll(255),cvScalarAll(0),100,2);
        }
        contours = contours->h_next;
    }
    Delete_Line_List(line_list_ancor);
    cvReleaseMemStorage(&storage);
}

void Calculate_Shape::Detect_Field_Borders(IplImage* frame)
{
    bool first_point = false;
    bool second_point = false;
    float slope = 0;
    Line* line_pointer;
    CvPoint point_a;
    CvPoint point_b;
    line_list_ancor = new Line;
    line_pointer = line_list_ancor;
    for(int x = 10; x < frame->width; x=x+10)
    {
        int y = 0;
        uchar* ptr = (uchar*) (frame->imageData + y * frame->widthStep);
        while((y < frame->height) && ((ptr[x*frame->nChannels + 1] != 255) || (Calculate_Precentage(frame,x,y,7) < 50)))
        {
            y = y+10;
            ptr = (uchar*) (frame->imageData + y * frame->widthStep);
        }
        if(second_point && first_point)
        {
            if(abs((-1*slope*(x - point_b.x) + -1*point_b.y) + y) < 12)
            {
                point_b.x = x;
                point_b.y = y;
                slope = ((float) (point_b.y - point_a.y))/ ((float)(point_b.x - point_a.x));
            }
            else
            {
                    cvLine( frame, point_a, point_b, CV_RGB(255,0,0), 3, CV_AA, 0 );
                    line_pointer->next = new Line;
                    line_pointer = line_pointer->next;
                    line_pointer->point1 = point_a;
                    line_pointer->point2 = point_b;
                    line_pointer->slope = slope;
                    line_pointer->next = NULL;
                    point_a.x = x;
                    point_a.y = y;
                    second_point = false;
            }
        }
        if(x != point_a.x)
        {
            if(!second_point && first_point)
            {
                point_b.x = x;
                point_b.y = y;
                slope = ((float) (point_b.y - point_a.y))/ ((float)(point_b.x - point_a.x));
                second_point = true;
            }
        }
        if(!first_point)
        {
            point_a.x = x;
            point_a.y = y;
            first_point = true;
        }
    }

    line_pointer->next = new Line;
    line_pointer = line_pointer->next;
    line_pointer->point1 = point_a;
    line_pointer->point2 = point_b;
    line_pointer->slope = slope;
    line_pointer->next = NULL;
    cvLine( frame, point_a, point_b, CV_RGB(255,0,0), 3, CV_AA, 0 );
}

Raw_Borders Calculate_Shape::Calculate_Raw_Borders(IplImage* frame)
{
    Line* line_pointer;
    CvRect rect;
    CvPoint point_a;
    CvPoint point_b;
    CvPoint temp;
    bool is_rect;
    bool is_left;

    is_rect = false;
    is_left = false;
    borders.is_left = false;
    borders.is_rect = false;
    rect.height = 0;
    rect.width = 0;
    rect.x = 1000;
    rect.y = 1000;
    point_a.x = 1000;
    point_a.y = -10;
    point_b.x = -10;
    point_b.y = 1000;

    line_pointer = line_list_ancor->next;
    if(line_pointer)
    {
        while(line_pointer)
        {
            if((abs(line_pointer->slope) < 0.1) && ((line_pointer->point2.x - line_pointer->point1.x) >= rect.width)
                    && (line_pointer->point1.y < rect.y)) 
            {
                if(line_pointer->point1.y <= line_pointer->point2.y)
                {
                    rect.y = line_pointer->point1.y;
                    rect.height = 480 - line_pointer->point1.y;
                }
                else
                {
                    rect.y = line_pointer->point2.y;
                    rect.height = 480 - line_pointer->point2.y;
                }
                rect.x = line_pointer->point1.x;
                rect.width = line_pointer->point2.x - line_pointer->point1.x;
                is_rect = true;
                borders.rect = rect;
                borders.is_rect = true;
            }
            else
            {
                if(line_pointer->slope < 0)
                {
                    if(line_pointer->point1.x <= point_a.x &&
                       line_pointer->point1.y >= point_a.y)
                    {
                        point_a.x = line_pointer->point1.x;
                        point_a.y = line_pointer->point1.y;
                    }
                    if(line_pointer->point2.x >= point_b.x &&
                       line_pointer->point2.y <= point_b.y)
                    {
                        point_b.x = line_pointer->point2.x;
                        point_b.y = line_pointer->point2.y;
                    }
                    is_left = true;
                    borders.left.point1 = point_a;
                    borders.left.point2 = point_b;
                    borders.left.slope = ((float) (point_b.y - point_a.y))/ ((float)(point_b.x - point_a.x));
                    borders.is_left = true;
                }
            }
            line_pointer = line_pointer->next;
        }
    }
    if(is_left)
    {
        if(!is_rect)
        {
            rect.x = point_b.x;
            rect.y = point_b.y;
        }
        temp.x = rect.x;
        temp.y = rect.y;
        cvLine( frame,temp, point_a, CV_RGB(0,0,255), 3, CV_AA, 0 );
    }
    if(is_rect)
        Draw_Box(frame, rect);
    return (borders);
    
}

CvPoint Calculate_Shape::Calculate_Center(CvSeq* contour)
{
    int max_distance = 0;
    int distance = 0;
    int count = 0;
    CvPoint point1,point2, middle_point;
    middle_point.x = 0;
    middle_point.y = 0;
    if(contour->total > 1)
    {
        for(int y = 0; y<contour->total; y++)
        {
            CvPoint* p1 = (CvPoint*)cvGetSeqElem ( contour, y );
            for(int x = contour->total - 1; x>y; x--)
            {
                CvPoint* p2 = (CvPoint*)cvGetSeqElem ( contour, x );
                distance = pow((p2->x - p1->x),2) + pow((p2->y - p1->y),2);
                if(max_distance < distance)
                {
                    max_distance = distance;
                    point1 = *p1;
                    point2 = *p2;
                }
            }
        }
        if(point1.x >= point2.x)
            middle_point.x = ((point1.x - point2.x) / 2) + point2.x;
        else
            middle_point.x = ((point2.x - point1.x) / 2) + point1.x;
        if(point1.y >= point2.y)
            middle_point.y = ((point1.y - point2.y) / 2) + point2.y;
        else
            middle_point.y = ((point2.y - point1.y) / 2) + point1.y;
        if(contour->total >= 50)
            return middle_point;
        else
        {
            middle_point.x = -5;
            middle_point.y = -5;
            return middle_point;
        }
       // for(int x = 0; x<contour->total; x++)
       // {
         //   CvPoint* p1 = (CvPoint*)cvGetSeqElem ( contour, x );
          //  if(abs((pow(p1->x - middle_point.x,2) + pow(p1->y - middle_point.y,2)) - max_distance) <= 1000)
            //    count++;
       // }
       // if(count / contour->total >= 0.5)
         //   return middle_point;
       // else
       // {
         //   middle_point.x = -5;
         //   middle_point.y = -5;
          //  return middle_point;
       // }
    }
}

CvPoint Calculate_Shape::Pixel_Pixel(IplImage* binnary_frame)
{

    IplImage* result_image = cvCreateImage( cvSize(binnary_frame->width,binnary_frame->height), binnary_frame->depth, 1 );
    int count;
    int distance;
    int max_distance1 = 0;
    int max_distance2 = 0;
    int result_y;
    int max = 0;
    float slope = 0;
    CvPoint max_point1;
    CvPoint max_point2;
    CvPoint draw_point;
    CvPoint max_point_result;
    CvPoint c;
    int vote[480][640];
    memset(vote,0,sizeof(vote));
    for( int y=0; y<binnary_frame->height; y++ )
    {
        uchar* ptr = (uchar*) (binnary_frame->imageData + y * binnary_frame->widthStep);
        for( int x=0; x<binnary_frame->width; x++ )
        {
            if(ptr[x*binnary_frame->nChannels] == 255)
            {
                count = 0;
                max_distance1 = 0;
                max_distance2 = 0;
                for(int index = y-15; index <= y+15; index++)
                {
                    if(index >= 0 && index < binnary_frame->height)
                    {
                        uchar* ptr2 = (uchar*) (binnary_frame->imageData + index * binnary_frame->widthStep);
                        for(int index2 = x-15; index2 <= x+15; index2++)
                        {
                            if(index2 >= 0 && index2 < binnary_frame->width )
                            {
                                if(ptr2[index2*binnary_frame->nChannels] == 255)
                                {
                                    distance = abs(index2 - x) + abs(index - y);
                                    if(distance > max_distance1)
                                    {
                                        max_point1.x = index2;
                                        max_point1.y = index;
                                        max_distance1 = distance;
                                    }
                                    else
                                    {
                                        if(distance > max_distance2)
                                        {
                                            max_point2.x = index2;
                                            max_point2.y = index;
                                            max_distance2 = distance;
                                        }
                                    }
                                    count++;
                                }
                            }
                        }
                    }
                }
                if(count == 31)
                {
                    if(max_point1.x - max_point2.x)
                        slope = -1*(max_point1.y - max_point2.y)/(max_point1.x - max_point2.x);
                    else
                        slope = -1*(max_point1.y - max_point2.y)/(0.0001);
                    c.x = (max_point1.x + max_point2.x)/2;
                    c.y = (max_point1.y + max_point2.y)/2;
                    if(-1*y != (int)((max_point1.y*-1) + slope*(x - max_point1.x)))
                    {
                        slope = slope*-1;
                        for(int index3 = 0; index3 < result_image->height; index3++)
                        {
                            uchar* ptr3 = (uchar*) (result_image->imageData + index3 * result_image->widthStep);
                            for(int index4 = 0; index4 < result_image->width; index4++)
                            {
                               result_y = (int)((y*-1) + slope*(index4 - x));
                               result_y = result_y * -1;
                               if(result_y == index3)
                               {
                                   ptr3[index4*result_image->nChannels] = 255;
                                   vote[index3][index4] = vote[index3][index4] + 1;
                                   if(vote[index3][index4] > max)
                                   {
                                       max_point_result.x = index4;
                                       max_point_result.y = index3;
                                       max = vote[index3][index4];
                                   }
                               }
                            }
                        }
                    }
                }
            }
        }
    }
    cvShowImage( "Pixel_Pixel", result_image);
    cvReleaseImage(&result_image);
    return(max_point_result);
}