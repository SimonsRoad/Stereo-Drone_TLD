#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <geometry_msgs/Twist.h>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  ros::Publisher cmdpub; 
  
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/downward_cam/camera/image", 1, 
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    cmdpub = nh_.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
   
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Angular movement for spiri
float angular(float posx){
float d1=posx-320;
float d2=map(d1, 0, 320, 0, 5);
if(d2>=-0.3 && d2<=0.3){
return(d2);
}
else if(d2<-0.3){
return(-0.3);
}
else{
return(0.3);
}
}

//Linear movement for spiri
float linear(float posy){
float d1=posy-240;
float d2=map(d1, 0, 240, 0, 5);
if(d2>=-0.3 && d2<=0.3){
return(d2);
}
else if(d2<-0.3){
return(-0.3);
}
else{return(0.3);
}
}


  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

        int nLowH = 0;
 	int nHighH = 179;

  	int nLowS = 18; 
 	int nHighS = 255;

  	int nLowV = 14;
 	int nHighV = 255;

    cv::Mat imgHSV;

    cv::cvtColor(cv_ptr->image, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    cv::Mat imgThresholded;

    cv::inRange(imgHSV, cv::Scalar(nLowH, nLowS, nLowV), cv::Scalar(nHighH, nHighS, nHighV), imgThresholded); //Threshold the image
		  
    //morphological opening (removes small objects from the foreground)
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
    cv::dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 

    //morphological closing (removes small holes from the foreground)
    cv::dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

    //Calculate the moments of the thresholded image
    cv::Moments oMoments = moments(imgThresholded);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

		//Threshold for tracking
		if (dArea > 10000)
	  	{
	   		//calculate the position of the ball (640X480)
	   		float posX = dM10 / dArea;
	   		float posY = dM01 / dArea;
			//std::cout << dArea << std::endl;    

                       float  vel_x=linear(posY);
                       float  vel_y=angular(posX);

                     geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
                     cmd->linear.x =vel_x;
                     cmd->angular.z =vel_y;
                     cmdpub.publish(cmd); 
                     ROS_INFO("x=%f,y=%f", cmd->linear.x, cmd->angular.z); 


                    if (vel_x == 0 && vel_y == 0)
			       {
				std::cout << "Object has been reached!" << std::endl;
				}
                        }
                 else
		{
			std::cout << "Lost..." << std::endl;
		
		}

              cv::imshow("Thresholded Image", imgThresholded);
              cv::imshow("Original", cv_ptr->image); 

          if (cv::waitKey(30) == 27)
            {
            std::cout << "Tracking has been stopped by user" << std::endl;
            return;
            }
 
  
}
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ros::NodeHandle nh;
  ImageConverter ic;
  ros::spin();
  return 0;
}
