#include <ros.h>
#include <std_msgs/Int32MultiArray.h>

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

void motor_callback(const std_msgs::Int32MultiArray& msg)
{
  power[0] = msg.data[0];
  power[1] = msg.data[1];
}

ros::Subscriber<std_msgs::Int32MultiArray> sub("row_power", &motor_callback);

void setup()
{
  nh.initNode();
  nh.advertise(row_count);
  nh.subscribe(sub);

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
  else if (power[0] == 0)
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
    digitalWrite(BIN_1, HIGH);
    digitalWrite(BIN_2, LOW);
  }
  else if (power[1] == 0)
  {
    digitalWrite(BIN_1, LOW);
    digitalWrite(BIN_2, LOW);
  }
  else if (power[1] < 0)
  {
    digitalWrite(BIN_1, LOW);
    digitalWrite(BIN_2, HIGH);
  }

  analogWrite(PWM_A, constrain(abs(power[0]), 20, 200));
  analogWrite(PWM_B, constrain(abs(power[1]), 20, 200));

  nh.spinOnce();

  delay(100);
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
