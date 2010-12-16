#include "Camera.h"
#include "Camera_Observer.h"
#include "Color_Detection.h"
#include "Video_Feed.h"
#include "Histogram.h"
#include "Threshold.h"

/*Description: this is the main function of the image processing section.
 * Function: the method will run an infinite loop of obtaining camera feed
 *           and will call for appropiate processing methods.
 * Parameters: None.
 * Return Value: 0 to operating system.*/
int main( int argc, char** argv)
{
    char c;
    Histogram hist;
    Threshold thresh;
    Camera* camera;
    camera = Camera::create_camera();
    Video_Feed video(camera);
    Color_Detection color_d(camera,&hist,&thresh);
    while(1)
    {
        camera->Get_frame();
        c = cvWaitKey(5);
    }
  return 0;
}
