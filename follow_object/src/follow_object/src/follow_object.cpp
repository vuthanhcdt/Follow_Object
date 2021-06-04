#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include "std_msgs/Float32MultiArray.h"
#include <visualization_msgs/MarkerArray.h> 

std::string topic_v,topic_pose_object,topic_pose_robot;
double x_object,y_object;
double pre_x_object,pre_y_object;
double x_g,y_g;
double L,R;
double van_toc_dai=0.3,van_toc_goc,v_max_xe;
double l=0.2;//diem cach robot nam tren duong thang
double dist_safe=1.5;
double rad_to_dr = 180/3.141592653589793238462643;//chuyen doi sang radian
double rad=3.14159265359/180;
double kp=2;
double goc_cho_phep=10;//10 do
double goc_tuong_doi,ss_goc;
double x_robot,y_robot,z_robot,w_robot,goc_robot;
double x_object_ref,y_object_ref;
ros::Publisher  pub_v;
ros::Publisher vis_pub;
geometry_msgs::Twist data; 
double pre_L;
ros::Time measurement_time,pre_measurement_time;
double dt;


//=============doc toa do cua doi tuong===================//
void read_pose_object(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  x_object=msg->data[0];
  y_object=msg->data[1];
}
void tracking(void)
{

  // dt=(measurement_time-pre_measurement_time).toSec();
  // van_toc_dai=(L-pre_L)/dt;
  // if(van_toc_dai<0.15) van_toc_dai=0.15;
  // else if(van_toc_dai>0.3) van_toc_dai=0.3;
  // pre_measurement_time=measurement_time;
  // pre_L=L;

  van_toc_dai=v_max_xe;
  if(y_object==0)
  {
      van_toc_goc=0;
  }
  else
  {
    
    //tinh toa do xg yg cach robot khoang l
    y_g=l*(y_object/L);
    R=(l*l)/(2*y_g);
    van_toc_goc=-van_toc_dai/R;
    ROS_INFO("%f,%f,%f,%f",y_g,R,van_toc_goc, van_toc_dai);
  }
}

void quaytaicho()
{
  van_toc_dai=0;
  van_toc_goc=-kp*ss_goc*rad;

  if(fabs(ss_goc) < goc_cho_phep)
  {
    van_toc_goc=0;
  }

}

void follow_object(void)
{
    measurement_time=ros::Time::now(); 
  
    L=sqrt(x_object*x_object+y_object*y_object);

    if(x_object!=0.0)
    {
        goc_tuong_doi=atan2(y_object,x_object)*rad_to_dr;
        ss_goc=goc_tuong_doi;
        
    }
    else
    {
        if(y_object>0)
        {
            goc_tuong_doi=90;
            ss_goc=90;
        }
        else if(y_object<0)
        {
            goc_tuong_doi=-90;
            ss_goc=-90;
        }
	else
	    ss_goc=0;
            
    }


   
    if(L<=dist_safe)
    {
        quaytaicho();

         
    }
    else
    {
        tracking();
    }
   
     //pub cmd_vel
    data.linear.x=van_toc_dai;
    data.angular.z=van_toc_goc;
    pub_v.publish(data);
   
}

void hien_thi_rviz(void)
{
    

   x_object_ref=x_robot+(cosf((goc_tuong_doi+goc_robot)*rad)*L);
   y_object_ref=y_robot+(sinf((goc_tuong_doi+goc_robot)*rad)*L);

   visualization_msgs::Marker marker;
   marker.header.frame_id = "odom";
   marker.header.stamp = ros::Time();
   marker.ns = "my_namespace";
   marker.id = 0;
   marker.type = visualization_msgs::Marker::CYLINDER;
   marker.action = visualization_msgs::Marker::ADD;

   marker.pose.position.x = x_object_ref;
   marker.pose.position.y = y_object_ref;
   marker.pose.position.z = 0;

//    marker.pose.orientation.x = 0.0;
//    marker.pose.orientation.y = 0.0;
//    marker.pose.orientation.z = 0.0;
//    marker.pose.orientation.w = 1.0;
   marker.scale.x = 0.3;
   marker.scale.y = 0.3;
   marker.scale.z = 0.3;
   marker.color.a = 1.0; // Don't forget to set the alpha!
   marker.color.r = 0.0;
   marker.color.g = 1.0;
   marker.color.b = 0.0;
   vis_pub.publish( marker );
}


void read_pose_robot(const nav_msgs::Odometry::ConstPtr& msg)
{

  x_robot=msg->pose.pose.position.x;
  y_robot=msg->pose.pose.position.y;
  z_robot=msg->pose.pose.orientation.z;
  w_robot=msg->pose.pose.orientation.w;
  goc_robot=atan2(2*w_robot*z_robot,1-2*z_robot*z_robot)*rad_to_dr;


  //bien doi he toa do tuong doi ve tuyet doi
}

int main(int argc, char ** argv)
{
  
  ros::init(argc, argv, "follow_object");
  ros::NodeHandle n;
  ros::NodeHandle private_node_handle("~");
  private_node_handle.param<std::string>("topic_v", topic_v, "cmd_vel");
  private_node_handle.param<std::string>("topic_pose_object", topic_pose_object, "pose_object");
  private_node_handle.param<std::string>("topic_pose_robot", topic_pose_robot, "odom");
  private_node_handle.param<double>("v_max_xe",v_max_xe,0.6);
  private_node_handle.param<double>("dist_safe",dist_safe,1);
  private_node_handle.param<double>("kp",kp,2);
  private_node_handle.param<double>("l",l,0.2);
  ros::Subscriber sub_object = n.subscribe(topic_pose_object, 10, read_pose_object);
  ros::Subscriber sub_robot = n.subscribe(topic_pose_robot, 10, read_pose_robot);
  pub_v = n.advertise<geometry_msgs::Twist>(topic_v, 10); 
  vis_pub = n.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );
  ros::Rate rate_sleep(30);

  while (ros::ok())
   {

    hien_thi_rviz();
    follow_object();
    rate_sleep.sleep();
    ros::spinOnce(); 
   }

}
