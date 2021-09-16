#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/Imu.h"
#include "iostream"
#include "rosbag/bag.h"
using namespace std;
int start = 0;
int finish = 0;
ros::Time duration;
ros::Time pre_time_lidar;
ros::Time new_time_lidar;
bool tag = false;
ros::Duration duration_time_lidar;
void cloud_Handler(const sensor_msgs::PointCloud2::Ptr &msg);
void image_L_Handler(const sensor_msgs::ImagePtr &msg);
void image_R_Handler(const sensor_msgs::ImagePtr &msg);
void imu_Handler(const sensor_msgs::Imu::Ptr &msg);


rosbag::Bag bag;


std::string IMAGE_TOPIC_L="/camera/infra1/image_rect_raw";
std::string IMAGE_TOPIC_R="/camera/infra2/image_rect_raw";
std::string pointCloudTopic="/lslidar_point_cloud";
std::string imuTopic="/camera/imu";

int main(int argc, char **argv)
{
	bag.open("/home/wwj/Datasets/ourdata/out-0901-03-01.bag",rosbag::bagmode::Write);
	
	ros::init(argc,argv,"listener");
	ros::NodeHandle nh;
	ros::Subscriber sub_image_L = nh.subscribe(IMAGE_TOPIC_L,30,image_L_Handler);
	ros::Subscriber sub_image_R = nh.subscribe(IMAGE_TOPIC_R,30,image_R_Handler);
	ros::Subscriber sub_lidar = nh.subscribe(pointCloudTopic,10,cloud_Handler);
	ros::Subscriber sub_imu = nh.subscribe(imuTopic,400,imu_Handler);
	ros::spin();
	return 0;
}
void cloud_Handler(const sensor_msgs::PointCloud2::Ptr &msg)
{
    
   	
	if (finish == 1)
	{
		new_time_lidar = ros::Time(duration.sec, msg->header.stamp.nsec);//不能直接乘0.000000001 否则结尾会多出0.000000098
		pre_time_lidar = msg->header.stamp;
		finish = 2;
		tag = true;
	}
	else if(finish == 2)
	{
		if ((msg->header.stamp - pre_time_lidar)>ros::Duration(0.0))
		{
			duration_time_lidar = msg->header.stamp - pre_time_lidar;
			pre_time_lidar = msg->header.stamp;
			new_time_lidar = new_time_lidar + duration_time_lidar;
		}
		else{
			cout<<"msg->header.stamp:     "<<msg->header.stamp<<endl;
			cout<<"duration_time_lidar:     "<<duration_time_lidar<<endl;
			pre_time_lidar = msg->header.stamp;
			new_time_lidar = new_time_lidar + duration_time_lidar;
		}
		tag = true;
		
	}
	if(tag == true)
	{
		
		msg->header.stamp = new_time_lidar;
		bag.write(pointCloudTopic,new_time_lidar,msg);
	}
	
	    
}
void image_L_Handler(const sensor_msgs::ImagePtr &msg)
{
	if(start == 0){
		duration = msg->header.stamp;
		start ++;
		finish ++;
	} 
	bag.write(IMAGE_TOPIC_L,msg->header.stamp,msg);
}
void image_R_Handler(const sensor_msgs::ImagePtr &msg)
{
	bag.write(IMAGE_TOPIC_R,msg->header.stamp,msg);
}
void imu_Handler(const sensor_msgs::Imu::Ptr &msg)
{
	bag.write(imuTopic,msg->header.stamp,msg);
}
