#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    for(int i = 0; i < img.height * img.step; ++i)	// Loop through each pixel in the image
    {
        if(img.data[i] == white_pixel)	// checks if there's a white pixel
	{
	    int column = i % img.step;	// calculates column where white pixel is

	    // img.step = img.width*3		WHOLE (length) IMAGE

	    if(i < (img.width / 3))	// checks if white pixel is in LEFT side
	    {
	        drive_robot(0.0, 0.5);	// move robot LEFT
		break;
	    }
	    else if((img.width / 3) < i < (2 * (img.width / 3)))	// checks if white pixel is in CENTER side
	    {
		drive_robot(0.5, 0.0);	// move robot FORWARD
		break;
	    }
	    else if((2 * (img.width / 3)) < i < img.step)	// checks if white pixel is in RIGHT side
	    {
		drive_robot(0.0, -0.5);	// move robot RIGHT
		break;
	    }
	}
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
