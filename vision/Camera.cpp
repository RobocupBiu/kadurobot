/* 
 * File:   Camera.cpp
 * Author: Dudi Likvornik & Yaron Naor
 * 
 * Created on November 21, 2010, 12:09 PM
 */

#include "Camera.h"
/*Description: This is an overload for the constructor.
 * Parameters: A CvCapture pointer .
 * Return Value: None.*/
Camera::Camera(CvCapture** new_capture)
{
    observers_list = new Camera_Observer;       //this is the ancor of the observer list.
    *new_capture = cvCreateCameraCapture( 1 ); //we will get a defualt camera as a resource.
}
/*Description: A default destructor. Will release the CvCapture pointer reasource.
 * Parameters: None.
 * Return Value: None.*/
Camera::~Camera()
{
    cvReleaseCapture( &capture );
}
/*Description: The singelton method.
 * create_camera will make sure we create a single instance of Camera.
 * Parameters: None.
 * Return Value: A pointer to our single instance (Camera*)*/
Camera* Camera::create_camera()
{
    if(camera_pointer)
        return (camera_pointer);
    else
    {
        camera_pointer = new Camera(&capture);
        return(camera_pointer);
    }
}
/*Description: Will return a frame. The following function will
 * query for a frame and will tell all the observers to update there list.
 * Parameters: None.
 * Return Value: None*/
void Camera::Get_frame()
{
    frame = cvQueryFrame( capture );                //we will query for a new frame from the camera device
    Camera_Observer* temp = observers_list->next;   //go to the first observer in the list
    while(temp)
    {
        temp->Update_frame(frame);                  //initiate the update method in every oberver on the list
        temp=temp->next;                            //go to the next observer on the list
    }
}
/*Description: Will attach a new observer to the list.
 * Parameters: The observer which we want to add.
 * Return Value: None.*/
void Camera::Observer_Attach(Camera_Observer* observer)
{
    Camera_Observer* temp = observers_list;         //get the ancor
    while(temp->next!=NULL)                         //move across the observer list
        temp = temp->next;
    observer->prev = temp;                          //add a new observer to the end of the list
    temp->next = observer;
    observer->next = NULL;
}
/*Description: Will detach a new observer to the list.
 * Parameters: The observer which we want to remove.
 * Return Value: None.*/
void Camera::Observer_Detach(Camera_Observer* observer)
{
    Camera_Observer* temp = observers_list;        //get the ancor
    while(temp != observer && temp != NULL)        //find the right observer
        temp = temp->next;
    temp->prev->next = temp->next;                 //remove the observer from the list
    temp->next->prev = temp->prev;
}

Camera* Camera::camera_pointer = NULL;            //associate space for static members
CvCapture* Camera::capture = NULL;                //associate space for static members
