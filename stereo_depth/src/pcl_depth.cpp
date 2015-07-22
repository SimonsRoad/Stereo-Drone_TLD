#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
    
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
//using namespace message_filters;
    
//void imgcb(const sensor_msgs::ImageConstPtr& msg)
    
//void callback(const sensor_msgs::ImageConstPtr& msg1, const PointCloud::ConstPtr& msg2)
void callback(const PointCloud::ConstPtr& msg2)
{ 
/*  
  cv_bridge::CvImagePtr im_ptr;
  im_ptr = cv_bridge::toCvCopy(msg1);
  cv::Rect rect;
  ROS_INFO("rect done");
  rect = cv::Rect(200,150,200,200);
  std::cout << "depth of depthmat: " << im_ptr->image.depth() << std::endl;
  std::cout << "channels of depthmat: " << im_ptr->image.channels() << std::endl;
  std::cout << "depthmat type: " << im_ptr->image.type() << std::endl;
  im_ptr->image = im_ptr->image(rect);
*/
// for pointcloud now
  int index = 1;
  int q = 1;
  int x = 1,y = 1;
  float z = 100;
  std::fstream file;
  file.open("123.txt");    
  int next =(msg2->width)*(msg2->height);
  file << "width=" << msg2->width << " height=" << msg2->height << " next=" << next << std::endl;
  std::cout << "width=" << msg2->width << " height=" << msg2->height << " next=" << next << std::endl;
  //cv::imshow("ROI",im_ptr->image);
  //cv::waitKey(1);


  



/*
while(index < next) 
{
   q=1;
   while(q<msg->width)
	{
	 //std::cout << "X= "<< x <<" Y="<<y<< std::endl;
         x++;
	 index++;
	 q++;	
	}
	//file << "The pixel is X=" << x << " Y=" << y <<" index="<< index << " q=" << q << std::endl;
        y++;  
        x=1;
	index++;
}
*/
file << "DONE---------------------------------" << std::endl; 
}
/*
 for(int index=1;index < next;index++)
     {
      int q=1;
      for(q; q < (msg->width); q++)    
	{
		std::cout << "X= "<< x <<" Y="<<y<< std::endl;
                x++;
	        index++;
              //  q++;
        }	      
	file << "The pixel is X=" << x << " Y=" << y <<" index="<< index << " q=" << q << std::endl;
        y++;  
        x=1;
}
file << "DONE---------------------------------" << std::endl; 
}
*/

/*


BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
  {
  if (!std::isnan(pt.x) && !std::isnan(pt.y) && !std::isnan(pt.z))
  {
  z=std::min(z,pt.z);  
  //file << "x="<< pt.x << " y=" << pt.y << " z=" << pt.z<<std::endl;
  //std::cout << " x="<< pt.x << " y=" << pt.y << " z=" << pt.z << std::endl;
  //sleep(1);
  }
 }
  file << "x="<< pt.x << " y=" << pt.y << " z=" << z<<std::endl;
  file << "reapeating the boost foreach now" << std::endl;
  std::cout << "reapeating the boost foreach now" << std::endl;
}
*/

int main (int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  //image_transport::ImageTransport it(nh);

//  message_filters::Subscriber<sensor_msgs::Image> sub(nh, "stereo/left/image_raw",10);
//  message_filters::Subscriber<sensor_msgs::PointCloud2> sub2(nh, "stereo/points2", 10);
 // TimeSynchronizer<sensor_msgs::Image, sensor_msgs::PointCloud2> sync(sub, sub2, 10);
//  sync.registerCallback(callback);

  ros::Subscriber sub = nh.subscribe<PointCloud>("stereo/points2", 10, callback);
  //ros::Subscriber sub2= it.subscribe("stereo/left/image_raw",10);
  ros::spin();
}


/*w
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  
  if (pcl::io::loadPCDFile<pcl::PointXYZ> ("test_pcd.pcd", *cloud) == -1) //* load the file
  {
    PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
    return (-1);
  }
  std::cout << "Loaded "
            << cloud->width * cloud->height
            << " data points from test_pcd.pcd with the following fields: "
            << std::endl;
  for (size_t i = 0; i < cloud->points.size (); ++i)
    std::cout << "    " << cloud->points[i].x
              << " "    << cloud->points[i].y
              << " "    << cloud->points[i].z << std::endl;

  return (0);
}
*/
