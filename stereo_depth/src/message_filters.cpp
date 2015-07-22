#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
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
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/QuaternionStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/Header.h>
#include <ctime>
#include <geometry_msgs/Twist.h>

//std::fstream file;
 // file.open("123.txt"); 

using namespace message_filters;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

ros::Publisher cmdpub;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float linearx(float z){
//float zout = map(z,1,2,0,1);
float target_depth = 1.0;
float zout = std::max(-1.0f, std::min(1.f, z - target_depth));
if (zout > 0 && zout < .2) zout = 0;
return zout;
}

float angularz(float cain){
float c1=160-cain;
float c2=map(c1, 0, 160, 0, 5);
if(c2>=-0.3 && c2<=0.3)
{
float c3=c2;
return(c3);
}
else if(c2<-0.3)
{
return(-0.3);
}
else{
return(0.3);
}}

float linearz(int clin){
float c4=120-clin;
float c5=map(c4, 0, 120, 0, 5);
if(c5>=-0.3 && c5<=0.3){
float c6=c5;
return(c6);
}
else if(c5<-0.3){
return(-0.3);
}
else
{
return(0.3);
}
}

void callback(const PointCloud::ConstPtr& cloud,const geometry_msgs::QuaternionStamped::ConstPtr& msgA) 
{
 time_t tstart, tend;
 tstart= time(0);
 ROS_INFO("It starts");


 int x1,y1,x2,y2,index;
 float x,z=1000,cq;
 std::cout<<"x1="<<msgA->quaternion.x<<" y1="<< msgA->quaternion.y<<" x2="<< msgA->quaternion.z<<" y2="<< msgA->quaternion.w<<std::endl;

if(std::isnan(msgA->quaternion.x)){
ROS_INFO("Object Lost!! Panic Mode!!");
geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
 cmd->linear.x = 0;    
 cmd->angular.z = 0;
 cmd->linear.z = 0;
 cmdpub.publish(cmd);
ROS_INFO("x=%f y=%f z=%f", cmd->linear.x, cmd->angular.z, cmd->linear.z);
std::cout<<"nan values received"<<std::endl;
}
else
{
x1= msgA->quaternion.x; y1= msgA->quaternion.y; x2= msgA->quaternion.z; y2= msgA->quaternion.w;
if(x1<1){x1=1;}if(y1<1){y1=1;}if(x2<1){x2=1;}if(y2<1){y2=1;}
if(x1>320){x1=320;}if(y1>240){x1=240;}if(x2>320){x2=320;}if(y2>240){y2=240;}
float width1=fabs(x1-x2), height1=fabs(y1-y2);

for(int m=x1;m<=x2;m++){
 for(int n=y1;n<=y2;n++){
  index = n*cloud->width+m;
   //std::cout<<"The index is:"<<index<<std::endl;
   if(!std::isnan(cloud->points[index].z)){
   cq=cloud->points[index].z;
   z=std::min(z,cq);
   }
 }
}
std::cout<<"z="<<z<<std::endl;
//std::cout<<"linear command="<<linearx(z)<<std::endl;

float width=fabs(x1-x2), height=fabs(y1-y2);
float cx=(x1+x2)/2, cy=(y1+y2)/2;
float area=width*height;
//ROS_INFO("area=%f", area);
float fc1=160, fc2=120;

geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
 cmd->linear.x = linearx(z);    
 cmd->angular.z = angularz(cx);     	
 cmd->linear.z = linearz(cy);
 cmdpub.publish(cmd); 
 ROS_INFO("x=%f y=%f z=%f", cmd->linear.x, cmd->angular.z, cmd->linear.z);
tend=time(0);
std::cout<<"time taken-> tstart="<<tstart<<" tend="<<tend<<std::endl;
}
}



int main(int argc, char** argv)
{
  ros::init(argc, argv, "pointsdepth");
  ROS_INFO("Spinning without entering");
  ros::NodeHandle nh;
  
  cmdpub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 5);
  message_filters::Subscriber<PointCloud> image_sub(nh, "stereo/points2", 10);
//message_filters::Subscriber<sensor_msgs::CameraInfo> pixelpoints(nh, "stereo/left/camera_info", 1);  
  message_filters::Subscriber<geometry_msgs::QuaternionStamped> pixelpoints(nh, "pointstampA", 10);
  //TimeSynchronizer<PointCloud, geometry_msgs::QuaternionStamped> sync(image_sub, pixelpoints, 1000);
  typedef sync_policies::ApproximateTime<PointCloud, geometry_msgs::QuaternionStamped> syncpolicy;
  Synchronizer<syncpolicy> sync(syncpolicy(20), image_sub, pixelpoints);
  sync.registerCallback(boost::bind(&callback, _1, _2));
  
  ros::spin();

  return 0;
}
