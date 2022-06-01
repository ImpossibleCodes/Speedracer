#include <Arduino.h>
#line 1 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
// Final Code, Kaushik
#include <Servo.h>
#include "src/TFMPlus.h"

static const int steeringPin = 32;
static const int escPin = 26;
static const int FRAMERATE = FRAME_1000;

TFMPlus tfmP_s;
TFMPlus tfmP_f;

Servo esc;
Servo steering;

#line 15 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
int speedCalc(int distance);
#line 24 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
void setup();
#line 96 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
void loop();
#line 15 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
int speedCalc(int distance)
{
  int temp = distance - 30;
  int speedVal = int(0.00008 * (distance - 30) * (distance - 30) + 1570);
  if (speedVal > 2000)
    return 2000;
  return speedVal;
}

void setup()
{

  esc.attach(escPin);
  steering.attach(steeringPin);
  steering.write(100);

  Serial.begin(115200); // Intialize terminal serial port
  delay(20);            // Give port time to initalize
  Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n");
  Serial1.begin(115200, SERIAL_8N1, 5, 18); // 5 , 18 .   22, 23 .   17, 16
  delay(20);
  Serial2.begin(115200, SERIAL_8N1, 22, 23);
  delay(20);              // Give port time to initalize
  tfmP_s.begin(&Serial1); // Initialize device library object and...
  tfmP_f.begin(&Serial2);

  Serial.println("Data-Frame rate: ");
  if (tfmP_s.sendCommand(SET_FRAME_RATE, FRAMERATE))
  {
    Serial.println(FRAMERATE);
  }
  else
    tfmP_s.printReply();

  delay(500);

  Serial.println("Data-Frame rate: ");
  if (tfmP_f.sendCommand(SET_FRAME_RATE, FRAMERATE))
  {
    Serial.println(FRAMERATE);
  }
  else
    tfmP_f.printReply();

  delay(500);
  if (tfmP_s.sendCommand(STANDARD_FORMAT_CM, 0))
  {
    Serial.println("CM");
  }
  else
    tfmP_s.printReply();

  delay(500);

  if (tfmP_f.sendCommand(STANDARD_FORMAT_CM, 0))
  {
    Serial.println("CM");
  }
  else
    tfmP_f.printReply();

  // - - - - - - - - - - - - - - - - - - - - - - - -
  delay(5000); // And wait for 5 seconds.
  esc.writeMicroseconds(1560);
}

// Initialize variables
int16_t tfDist_l = 0; // Distance to object in centimeters
int16_t tfFlux_l = 0; // Strength or quality of return signal
int16_t tfTemp_l = 0; // Internal temperature of Lidar sensor chip
int16_t tfDist_r = 0; // Distance to object in centimeters
int16_t tfFlux_r = 0; // Strength or quality of return signal
int16_t tfTemp_r = 0; // Internal temperature of Lidar sensor chip
int16_t tfDist_f = 0; // Distance to object in centimeters
int16_t tfFlux_f = 0; // Strength or quality of return signal
int16_t tfTemp_f = 0; // Internal temperature of Lidar sensor chip
int16_t maxDist = 0;  // maximum distance found by sensors
int steeringVal = 100;
int speedVal = 1565;
bool left = true; // toggle for reading

void loop()
{

  Serial1.end();
  delay(10);
  Serial1.flush();

  if (!left)
  {
    left = true;
    Serial1.begin(115200, SERIAL_8N1, 5, 18);
  }
  else
  {
    left = false;
    Serial1.begin(115200, SERIAL_8N1, 17, 16);
  }

  if (left)
  {
    if (!tfmP_s.getData(tfDist_l, tfFlux_l, tfTemp_l))
    {
      tfmP_s.printFrame();
    }
  }
  else
  {
    if (!tfmP_s.getData(tfDist_r, tfFlux_r, tfTemp_r))
    {
      tfmP_s.printFrame();
    }
  }

  if (!tfmP_f.getData(tfDist_f, tfFlux_f, tfTemp_f))
  {
    tfDist_f = 100;
  }

  // steering code
  if (tfDist_f > tfDist_l && tfDist_f > tfDist_r)
  {

    Serial.println("F");
    maxDist = tfDist_f;
    steeringVal = 100;

    if (tfDist_r < 20)
    {
      steeringVal = 90;
    }
    else if (tfDist_l < 20)
    {
      steeringVal = 110;
    }
  }
  else if (tfDist_r > tfDist_l)
  {

    Serial.println("R");
    maxDist = tfDist_r;
    steeringVal = 180;
  }
  else
  {

    Serial.println("L");
    maxDist = tfDist_l;
    steeringVal = 70;
  }
  int threshold = (int)((0.1379 * speedVal) - 100);
  if (tfDist_f < threshold)
  {

    Serial.println("REVERSE");
    steeringVal = -steeringVal;
    speedVal = 1300;
  }
  else
  {

    speedVal = speedCalc(maxDist);
  }

  esc.writeMicroseconds(speedVal);
  steering.write(steeringVal);
}

