/*
 * File:   Color_Detection.cpp
 * Author: Dudi Likvornik & Yaron Naor
 *
 * Created on November 21, 2010, 3:00 PM
 */
#include "Color_Detection.h"

/*Description: The function will get a mouse event.
 *             The function will get a specific mouse event, the location of the mouse
 *             and a specific flag from the computer mouse.
 * Parameters: event - the current mouse event, x - mouse x location by image coor,
 *             y - mouse y location by image coor, flags - the current mouse flags,
 *             param - a general resource.
 * Return Value: None.*/
void Color_Detection::Mouse_event(int event, int x, int y, int flags, void* param)
{
    switch( event )                 //check for the current event
    {
        case CV_EVENT_MOUSEMOVE:    //if the mouse is moving
        {
            if( drawing_box )       //check if we want to draw a box
            {
                box.width = x-box.x;    //calculate the box width
                box.height = y-box.y;   //calculate the box height
            }
        }
        break;
        case CV_EVENT_LBUTTONDOWN:      //left button is pressed
        {
            drawing_box = true;         //we want to draw a box
            box = cvRect(x, y, 0, 0);   //create a new box
        }
        break;
        case CV_EVENT_LBUTTONUP:        //if we don't press the left button
        {
            drawing_box = false;        //don't draw a new box
            if(box.width<0)             //if the width is negative
            {
                box.x+=box.width;       //move x to the left
                box.width *=-1;         //the width is now positive
            }
            if(box.height<0)            //if the height is negative
            {
                box.y+=box.height;      //move y up
                box.height*=-1;         //the height is now positive
            }
        }
        break;
    }
}
/*Description: This is a wrapper function for Mouse_Event.
 * Parameters: event - the current mouse event, x - mouse x location by image coor,
 *             y - mouse y location by image coor, flags - the current mouse flags,
 *             param - a general resource.
 * Return Value: None.*/
void Color_Detection::Mouse_event_Wrapper(int event, int x, int y, int flags, void* param)
{
    Color_Detection* temp_color_detection = (Color_Detection*) param;
    temp_color_detection->Mouse_event(event,x,y,flags,NULL);
}
/*Description: Defualt constructor.
 *             The constructor will attach the current instance as a new observer,
 *             and will initialize our box and drawing bool.
 * Parameters: event - The camera we are using.
 * Return Value: None.*/
Color_Detection::Color_Detection(Camera* new_camera, Histogram* new_hist, Threshold* new_threshold)
{
    camera = new_camera;            //get the camera
    camera->Observer_Attach(this);  //attach me to the observer list
    drawing_box = false;            //don't draw a box yet
    box = cvRect(-1,-1,0,0);        //create a false box
    hist = new_hist;                //get the histogram
    threshold = new_threshold;      //get the threshold
    for(int index = 0; index < 3; index++)  //reset the statistic mean and var
    {
        result.mean[index] = -100;
        result.var[index] = -100;
    }
    cvNamedWindow( "Color_HSV_Segmentation", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Pixel_Pixel", CV_WINDOW_AUTOSIZE );
}
/*Description: Destructor.
 *             The destructor will dettach the current instance from the observers list,
 *             and will release the window resource.
 * Parameters: None.
 * Return Value: None.*/
Color_Detection::~Color_Detection()
{
    camera->Observer_Detach(this);      //remove me from the observer list
    cvDestroyWindow("Color_HSV_Segmentation");
     cvDestroyWindow("Pixel_Pixel");
}
/*Description: The function will get a new frame and will do segmentation
 * Parameters: event - The camera we are using.
 * Return Value: None.*/

void Color_Detection::Update_frame(IplImage* new_frame)
{
        frame = cvCreateImage( cvSize(new_frame->width,new_frame->height), new_frame->depth, new_frame->nChannels );    //create a new image
        cvCopyImage(new_frame,frame);                                                       //copy the new image
        HSV_Segmentation(frame);                                                            //do HSV segmentation
        cvReleaseImage(&frame);                                                             //release the copied frame

}
/*Description: The function will draw a box in an image.
 * Parameters: frame - the frame we are going to draw our box in,
 *             rect - the box.
 * Return Value: None.*/
void Color_Detection::Draw_box(IplImage* frame, CvRect rect)
{
    cvRectangle(
            frame,
            cvPoint(rect.x,rect.y),
            cvPoint(rect.x + rect.width, rect.y + rect.height),
            cvScalar(255,0,0),
            2);
}
/*Description: The function will do a LPF on an image using gauss filter.
 * Parameters: frame - the frame we are going to smooth,
 * Return Value: None.*/
void Color_Detection::Smooth(IplImage** frame)
{
    //seperate the image into 3 basic channels
    IplImage* red = cvCreateImage( cvSize((*frame)->width,(*frame)->height), (*frame)->depth, 1 );
    IplImage* blue = cvCreateImage( cvSize((*frame)->width,(*frame)->height), (*frame)->depth, 1 );
    IplImage* green = cvCreateImage( cvSize((*frame)->width,(*frame)->height), (*frame)->depth, 1 );
    cvSplit(*frame,blue,green,red,NULL);
    //smooth each one
    cvSmooth(blue,blue,CV_GAUSSIAN,7,7,1);
    cvSmooth(green,green,CV_GAUSSIAN,7,7,1);
    cvSmooth(red,red,CV_GAUSSIAN,7,7,1);
    cvMerge(blue,green,red,NULL,*frame);            //merge them into one image
    cvReleaseImage(&red);                           //release the copied frame
    cvReleaseImage(&green);                         //release the copied frame
    cvReleaseImage(&blue);                          //release the copied frame
}
/*Description: The function will do segmentation using the HSV color system.
 *             it will create simple threshold to the hue of orange, yellow and green.
 * Parameters: frame - the frame we are going to segment.
 * Return Value: None.*/
void Color_Detection::HSV_Segmentation(IplImage* frame)
{
    CvRect box;
    CvPoint point;
    Calculate_Shape shape;              //will be used to detect shapes
    IplImage* duplicate_frame = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, frame->nChannels );         //copy the frame
    IplImage* duplicate_RGB_frame = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, frame->nChannels );     //create an RGB frame
    IplImage* duplicate_gray_frame = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1 );                   //create a monochronic frame
    IplImage* hue = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1 );                                    //create a hue frame
    IplImage* saturation = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1 );                             //create the saturation frame
    IplImage* value = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1 );                                  //create the value frame
    cvCvtColor(frame,duplicate_frame,CV_BGR2HSV);               //convert the frame into HSV
    cvSplit(duplicate_frame,hue,saturation,value,NULL);         //get all three channels
    for( int y=0; y<hue->height; y++ )
    {
        uchar* ptr = (uchar*) (hue->imageData + y * hue->widthStep);
        uchar* ptr2 = (uchar*) (saturation->imageData + y * saturation->widthStep);
        uchar* ptr3 = (uchar*) (value->imageData + y * value->widthStep);
        uchar* grayptr = (uchar*) (duplicate_gray_frame->imageData + y * duplicate_gray_frame->widthStep);
        for( int x=0; x<hue->width; x++ )
        {
            //for yellow
            if(ptr[x] >= 22 && ptr[x] <= 40 && ptr2[x] >= 100)
            {
                    ptr[x] = 30;
                    ptr2[x] = 255;
                    ptr3[x] = 255;
                    grayptr[x] = 0;
            }
            else
            {
                //for green
               if(ptr[x] <= 95 && ptr[x] >= 45 && ptr2[x] >= 120)
               {
                    ptr[x] = 60;
                    ptr2[x] = 255;
                    ptr3[x] = 255;
                    grayptr[x] = 0;
               }
               else
               {
                   //for red
                   if(ptr[x] >= 0 && ptr[x] < 22 && ptr2[x] >= 100)
                   {
                       ptr[x] = 19;
                       ptr2[x] = 255;
                       ptr3[x] = 255;
                       grayptr[x] = 255;
                   }
                   else
                   {
                       //make the rest black
                        ptr[x] = 0;
                        ptr2[x] = 0;
                        ptr3[x] = 0;
                        grayptr[x] = 0;
                   }
               }
            }
        }
    }
    cvMerge(hue,saturation,value,NULL,duplicate_frame);                     //merge the seperate channels
    cvCvtColor(duplicate_frame,duplicate_RGB_frame,CV_HSV2BGR);             //convert the image into RGB
    //cvCanny( duplicate_gray_frame, duplicate_gray_frame, 50, 200, 3 );
    shape.Calculate_Contours(duplicate_RGB_frame,duplicate_gray_frame);     //get the contours
    cvShowImage( "Color_HSV_Segmentation", duplicate_RGB_frame);               //show the image
    cvReleaseImage(&hue);                                                   //release the copied frame
    cvReleaseImage(&saturation);                                            //release the copied frame
    cvReleaseImage(&value);                                                 //release the copied frame
    cvReleaseImage(&duplicate_frame);                                       //release the copied frame
    cvReleaseImage(&duplicate_RGB_frame);                                   //release the copied frame
    cvReleaseImage(&duplicate_gray_frame);                                  //release the copied frame
}
                                                             