#include <ros.h>
#include <std_msgs/Int32MultiArray.h>
#define PULSE_MA_A 5
#define PULSE_MA_B 3
#define PULSE_MB_A 4
#define PULSE_MB_B 2

ros::NodeHandle  nh;
std_msgs::Int32MultiArray array_msg;
ros::Publisher row_count("row_count_MA", &array_msg);

int count[2] = 0;

void setup()
{
  nh.initNode();
  nh.advertise(row_count);

  pinMode(PULSE_MA_A, INPUT);
  pinMode(PULSE_MA_B, INPUT);
  pinMode(PULSE_MB_A, INPUT);
  pinMode(PULSE_MB_B, INPUT);

  attachInterrupt(1, callback_MA, CHANGE);
  attachInterrupt(0, callback_MB, CHANGE);
}

void loop()
{
  array_msg.data[0] = count[0];
  array_msg.data[1] = count[1];
  nh.spinOnce();
}

void callback_MA(void) {  
  if     ((digitalRead(PULSE_MA_A) == true) && (digitalRead(PULSE_MA_B) == true)) {
    count[0]++;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MA_A) == true) && (digitalRead(PULSE_MA_B) == false)) {
    count[0]--;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MA_A) == false) && (digitalRead(PULSE_MA_B) == true)) {
    count[0]--;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MA_A) == false) && (digitalRead(PULSE_MA_B) == false)) {
    count[0]++;
    row_count.publish( &array_msg );
  }
}

void callback_MB(void) {  
  if     ((digitalRead(PULSE_MB_A) == true) && (digitalRead(PULSE_MB_B) == true)) {
    count[1]++;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MB_A) == true) && (digitalRead(PULSE_MB_B) == false)) {
    count[1]--;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MB_A) == false) && (digitalRead(PULSE_MB_B) == true)) {
    count[1]--;
    row_count.publish( &array_msg );
  }
  else if((digitalRead(PULSE_MB_A) == false) && (digitalRead(PULSE_MB_B) == false)) {
    count[1]++;
    row_count.publish( &array_msg );
  }
}
