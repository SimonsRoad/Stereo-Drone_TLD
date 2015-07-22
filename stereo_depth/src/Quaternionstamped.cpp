#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/QuaternionStamped.h>
#include "std_msgs/Header.h"

ros::Subscriber sub;
ros::Publisher pub;

void stamped(const geometry_msgs::Quaternion::ConstPtr& msg)
{
ROS_INFO("it starts now");

geometry_msgs::QuaternionStamped pointstampA;

pointstampA.header.stamp = ros::Time::now();
std::cout<<"Quaternion header stamp="<<pointstampA.header.stamp<<std::endl;
pointstampA.quaternion.x= msg->x;
pointstampA.quaternion.y= msg->y;
pointstampA.quaternion.z= msg->z;
pointstampA.quaternion.w= msg->w;
pub.publish(pointstampA);

}


int main(int argc ,char** argv)
{
ros::init(argc,argv, "stamp");
ros::NodeHandle nh;
sub = nh.subscribe("/pointA",5,stamped);
pub = nh.advertise<geometry_msgs::QuaternionStamped>("/pointstampA",5);
ros::spin();
}

