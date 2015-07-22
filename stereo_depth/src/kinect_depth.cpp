#include <iostream>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <boost/foreach.hpp>

image_transport::Subscriber sub;
image_transport::Publisher pub;

void imgcb(const sensor_msgs::ImageConstPtr& msg)
{
ROS_INFO("begin imgcb");
cv_bridge::CvImagePtr im_ptr;
im_ptr = cv_bridge::toCvCopy(msg);
cv::Rect rect;
ROS_INFO("rect done");
rect = cv::Rect(200,150,200,200);
std::cout << "depth of depthmat: " << im_ptr->image.depth() << std::endl;
std::cout << "channels of depthmat: " << im_ptr->image.channels() << std::endl;
std::cout << "depthmat type: " << im_ptr->image.type() << std::endl;
im_ptr->image = im_ptr->image(rect);
unsigned char *ip = (unsigned char*) (im_ptr->image.data);
int i,j,c,d,e;
d =1e6;

for(int i = 0;i < im_ptr->image.rows;i++){
    for(int j = 0;j < im_ptr->image.cols;j++){
       // std::cout << " x="<< ip[im_ptr- << " y=" << ip.y << " z=" << ip.z << std::endl;
        c = ip[im_ptr->image.cols * j + 3 ] ;
	d=std::min(c,d);
	e=std::max(c,d);
        }
  }

pub.publish(im_ptr->toImageMsg());
std::cout << "depth in roi: " << c << std::endl;
std::cout << "min depth in ROI: " << d << std::endl;
std::cout << "max depth in ROI: " << e << std::endl;

std::cout <<"------"<<std::endl;
cv::imshow("ROI",im_ptr->image);
cv::waitKey(1);

}

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "foo");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    sub = it.subscribe("/my_iage", 1, imgcb);
    pub = it.advertise("/kill/depth", 1);
    //cv::namedWindow("foo");
    ros::spin();
    //cv::destroyWindow("foo");
    return 0;
}


