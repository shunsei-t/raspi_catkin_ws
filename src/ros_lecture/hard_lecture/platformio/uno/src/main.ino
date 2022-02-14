#include <ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msdgs/Twist.h>

#define STATE 11
#define PWM_A 10
#define AIN_1 13
#define AIN_2 12
#define PWM_B 9
#define BIN_1 7
#define BIN_2 8

#define PULSE_MA_A 5
#define PULSE_MA_B 3
#define PULSE_MB_A 4
#define PULSE_MB_B 2

ros::NodeHandle  nh;
std_msgs::Int32MultiArray array_msg;
ros::Publisher row_count("row_count", &array_msg);

long int count[2] = {0}; //A, B
int power[2] = {0}; //A, B

void motor_callback(const geometry_msgs::Twist& cmd_vel)
{
  power[0] = cmd_vel.linear.x * 25;
  power[1] = cmd_vel.linear.x * 25;
  power[0] = cmd_vel.angular.z * 10;
  power[1] = cmd_vel.angular.z * (-10);
}

ros::Subscriber<geometry_msgs::Twist> sub0("cmd_vel", &motor_callback);

void setup()
{
  nh.initNode();
  nh.advertise(row_count);
  nh.subscribe(sub0);

  array_msg.data = (long int*)malloc(sizeof(int)*2); //array_msg.data.resizeが使えないのでこうする
  array_msg.data_length = 2;

  pinMode(PULSE_MA_A, INPUT);
  pinMode(PULSE_MA_B, INPUT);
  pinMode(PULSE_MB_A, INPUT);
  pinMode(PULSE_MB_B, INPUT);

  pinMode(STATE, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(AIN_1, OUTPUT);
  pinMode(AIN_2, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  pinMode(BIN_1, OUTPUT);
  pinMode(BIN_2, OUTPUT);

  digitalWrite(STATE, HIGH);

  attachInterrupt(1, callback_MA, CHANGE);
  attachInterrupt(0, callback_MB, CHANGE);
}

void loop()
{
  array_msg.data[0] = count[0];
  array_msg.data[1] = count[1];

  if (power[0] > 0)
  {
    digitalWrite(AIN_1, HIGH);
    digitalWrite(AIN_2, LOW);
  }
  else if (power[0] = 0)
  {
    digitalWrite(AIN_1, LOW);
    digitalWrite(AIN_2, LOW);
  }
  else if (power[0] < 0)
  {
    digitalWrite(AIN_1, LOW);
    digitalWrite(AIN_2, HIGH);
  }
  
  if (power[1] > 0)
  {
    digitalWrite(BIN_1, LOW);
    digitalWrite(BIN_2, HIGH);
  }
  else if (power[1] = 0)
  {
    digitalWrite(BIN_1, LOW);
    digitalWrite(BIN_2, LOW);
  }
  else if (power[1] < 0)
  {
    digitalWrite(BIN_1, HIGH);
    digitalWrite(BIN_2, LOW);
  }

  analogWrite(PWM_A, power[0]);
  analogWrite(PWM_B, power[1]);

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
