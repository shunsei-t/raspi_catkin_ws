#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "basic_param_talker");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 10);
  std::string msg_chatter = "hello world!";
  pnh.getParam("content", msg_chatter);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    std_msgs::String msg;
    msg.data = msg_chatter;
    ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
