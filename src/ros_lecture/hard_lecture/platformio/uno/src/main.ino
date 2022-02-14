#include <ros.h>
#include <std_msgs/Int16.h>
#define MA_PULSE_A 5
#define MA_PULSE_B 3
#define MB_PULSE_A

ros::NodeHandle  nh;
std_msgs::Int16 int_msg;
ros::Publisher row_count("row_count", &int_msg);
int count = 0;

void setup()
{
  nh.initNode();
  nh.advertise(row_count);

  pinMode(PULSE_A, INPUT);
  pinMode(PULSE_B, INPUT);

  attachInterrupt(0, callbask, CHANGE);
}

void loop()
{
  int_msg.data = count;
  nh.spinOnce();
}

void callbask(void) {  
  if     ((digitalRead(PULSE_A) == true) && (digitalRead(PULSE_B) == true)) {
    count++;
    row_count.publish( &int_msg );
  }
  else if((digitalRead(PULSE_A) == true) && (digitalRead(PULSE_B) == false)) {
    count--;
    row_count.publish( &int_msg );
  }
  else if((digitalRead(PULSE_A) == false) && (digitalRead(PULSE_B) == true)) {
    count--;
    row_count.publish( &int_msg );
  }
  else if((digitalRead(PULSE_A) == false) && (digitalRead(PULSE_B) == false)) {
    count++;
    row_count.publish( &int_msg );
  }
}
