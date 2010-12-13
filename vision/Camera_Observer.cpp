/* 
 * File:   Camera_Observer.cpp
 * Author: Dudi Likvornik & Yaron Naor
 * 
 * Created on November 21, 2010, 2:01 PM
 */
#include "Camera_Observer.h"
/*Description: The defualt constructor of Camera_Observer.
 *             Will nolify the next and prev pointers.
 * Parameters: None.
 * Return Value: None.*/
Camera_Observer::Camera_Observer()
{
    next = NULL;
    prev = NULL;
}
/*Description: The destructor of Camera_Observer.
 * Parameters: None.
 * Return Value: None.*/
Camera_Observer::~Camera_Observer()
{
}
/*Description: virtual prototype of update.
 * Parameters: the frame we recieved in the update.
 * Return Value: None.*/
void Camera_Observer::Update_frame(IplImage*)
{
}

