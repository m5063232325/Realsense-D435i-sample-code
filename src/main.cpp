#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"
#include "opencv2/highgui/highgui.hpp"
#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.h"

bool opencv = 0;
void imageCallBack(const sensor_msgs::ImageConstPtr& img){
    ROS_INFO("[CAMERA INFO]: I heard a image data: %s", img->header.frame_id.c_str());
    ROS_INFO("[CAMERA INFO]: height, width, step: %d, %d, %d", img->height, img->width, img->step);
    sensor_msgs::Image rgb_msg;
    rgb_msg = *img;
    if(opencv)
    {
        try
        {
            cv::imshow("view", cv_bridge::toCvShare(img, "bgr8")->image);
            cv::waitKey(30);
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not convert from '%s' to 'bgr8'.", img->encoding.c_str());
        }
    }
    else
    {
        ROS_INFO("[CAMERA INFO]: The first pixel of image: %d", rgb_msg.data[0]);
        // for(int i = 0; i < (img->step * img->height); i++)
        // {
        //     ROS_INFO("[CAMERA INFO]: The %d pixel of image: %d", i, rgb_msg.data[i]);
        // }
    }
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    if(argc > 1)
    {
        opencv = (strcmp(argv[1], "true") == 0);
    }
    
    if(opencv)
    {
        cv::namedWindow("view");
        image_transport::ImageTransport it(n);
        image_transport::Subscriber img_sub = it.subscribe("/camera/color/image_raw", 1000, imageCallBack);
        ros::spin();
        cv::destroyWindow("view");
    }
    else
    {
        ros::Subscriber img_sub = n.subscribe("/camera/color/image_raw", 1000, imageCallBack);
        ros::spin();
    }
    return 0;
}
