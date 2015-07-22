#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <fstream>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <pcl_ros/point_cloud.h>
#include <pcl/visualization/cloud_viewer.h>
#include <ros/ros.h>
#include <math.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <geometry_msgs/Quaternion.h>

//using namespace sensor_msgs;
using namespace message_filters;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& cloud, const sensor_msgs::CameraInfoConstPtr& cam_info)
{
 int x1,y1,x2,y2,index;
 float x,z=1000,cq;

 //x1= msgA->x;
 //y1= msgA->y; 
 //x2= msgA->z;
 //y2= msgA->w;
 x1=50; y1=20; x2=150; y2=120;
 //cloud->points.resize (cloud->width * cloud->height);
  //std::fstream file;
 // file.open("123.txt"); 
std::cout<<"cloud width="<<cloud->width<<" Cloud Height="<<cloud->height<<std::endl;
sleep(1);

  for(int m=x1;m<=x2;m++)
  {
    for(int n=y1;n<=y2;n++)
     {
      index = n*cloud->width+m;
      if(!std::isnan(cloud->points[index].z)){
      cq=cloud->points[index].z;
      z=std::min(z,cq);
     }
   }
}
std::cout<<"z="<<z<<std::endl;
std::cout<< "camera_info...Width="<<cam_info->width<<" Height="<<cam_info->height<<std::endl;
std::cout<<"cloud header stamp"<<cloud->header.stamp<<std::endl;
/*
float width=fabs(x1-x2), height=fabs(y1-y2);
float cx=(x1+x2)/2, cy=(y1+y2)/2;
float area=width*height;
ROS_INFO("area=%f", area);
float fc1=160, fc2=120;

if(area!=area){
ROS_INFO("Object Lost!! Panic Mode!!");
geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
 cmd->linear.x = 0;    
 cmd->angular.z = 0;
 cmd->linear.z = 0;
 cmdpub.publish(cmd);
ROS_INFO("x=%f y=%f z=%f", cmd->linear.x, cmd->angular.z, cmd->linear.z);
}

else{
geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
 cmd->linear.x = linearx(area);    
 cmd->angular.z = angularz(cx);     	
 cmd->linear.z = linearz(cy);
 cmdpub.publish(cmd); 
 ROS_INFO("x=%f y=%f z=%f", cmd->linear.x, cmd->angular.z, cmd->linear.z);
}
*/
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "pointsdepth");

  ros::NodeHandle nh;

  message_filters::Subscriber<PointCloud> image_sub(nh, "stereo/points2", 1);
  message_filters::Subscriber<sensor_msgs::CameraInfo> pixelpoints(nh, "stereo/left/camera_info", 1);
  TimeSynchronizer<PointCloud, sensor_msgs::CameraInfo> sync(image_sub, pixelpoints, 10);
  sync.registerCallback(boost::bind(&callback, _1, _2));
  
  ros::spin();

  return 0;
}
