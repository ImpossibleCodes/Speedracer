# 1 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino"
# 2 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino" 2
# 3 "/Users/neilagrawal/Projects/Speedracer/Final3Point/Final3Point.ino" 2

static const int steeringPin = 32;
static const int escPin = 26;
static const int FRAMERATE = 0x03E8;

TFMPlus tfmP_r;
TFMPlus tfmP_l;
TFMPlus tfmP_f;

Servo esc;
Servo steering;

int speedCalc(int distance)
{
  int temp = distance - 30;
  int speedVal = int(0.00004 * (distance - 30) * (distance - 30) + 1565);
  if (speedVal > 2000)
    return 2000;
  return speedVal;
}

void setup()
{

  esc.attach(escPin);
  steering.attach(steeringPin);
  steering.write(100);

  delay(20); // Give port time to initalize
  Serial1.begin(115200, 0x800001c, 5, 18); // 5 , 18 .   22, 23 .   17, 16
  delay(20);
  Serial2.begin(115200, 0x800001c, 22, 23);
  delay(20);
  // Give port time to initalize
  Serial.begin(115200, 0x800001c, 17, 16);
  delay(20);
  tfmP_r.begin(&Serial);
  tfmP_l.begin(&Serial1); // Initialize device library object and...
  tfmP_f.begin(&Serial2);

  if (tfmP_r.sendCommand(0x00030606 /* These commands each return*/, FRAMERATE))
  {
    Serial.println(FRAMERATE);
  }
  else
    tfmP_r.printReply();

  delay(500);

  if (tfmP_f.sendCommand(0x00030606 /* These commands each return*/, FRAMERATE))
  {
    Serial.println(FRAMERATE);
  }
  else
    tfmP_f.printReply();

  if (tfmP_l.sendCommand(0x00030606 /* These commands each return*/, FRAMERATE))
  {
    Serial.println(FRAMERATE);
  }
  else
    tfmP_l.printReply();

  delay(500);

  delay(500);
  if (tfmP_r.sendCommand(0x01050505 /*           "*/, 0))
  {
  }
  else
    tfmP_r.printReply();

  delay(500);

  if (tfmP_f.sendCommand(0x01050505 /*           "*/, 0))
  {
  }
  else
    tfmP_f.printReply();

  delay(500);

  if (tfmP_l.sendCommand(0x01050505 /*           "*/, 0))
  {
  }
  else
    tfmP_l.printReply();

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
int16_t maxDist = 0; // maximum distance found by sensors
int steeringVal = 100;
int speedVal = 1565;
bool left = true; // toggle for reading

void loop()
{

  if (!tfmP_l.getData(tfDist_l, tfFlux_l, tfTemp_l))
  {
    tfDist_l = 100;
  }
  if (!tfmP_r.getData(tfDist_r, tfFlux_r, tfTemp_r))
  {
    tfDist_r = 100;
  }

  if (!tfmP_f.getData(tfDist_f, tfFlux_f, tfTemp_f))
  {
    tfDist_f = 100;
  }

  // steering code
  steeringVal = 55 * tfDist_l + 100 * tfDist_f + 145 * tfDist_r;
  steeringVal = steeringVal / (tfDist_l + tfDist_f + tfDist_r);

  if(steeringVal >= 130) {
    steeringVal = 130;
  }
  else if(steeringVal <= 70) {
    steeringVal = 70;
  }

  if(tfDist_r > tfDist_l + 5 && tfDist_f < 50) {
    steeringVal = 130;
  } else if (tfDist_l > tfDist_r + 5 && tfDist_f < 50) {
    steeringVal = 70;
  }
  // steeringVal = steeringVal + 100;

  int threshold = (int)((0.1 * speedVal) - 115);
  if (tfDist_f < threshold)
  {

    steeringVal = -steeringVal;
    speedVal = 1380;
  }
  else
  {

    speedVal = speedCalc(maxDist);
    // if(tfDist_f > 110) {
    //   speedVal = 2000;
    // }
  }

  esc.writeMicroseconds(speedVal);
  steering.write(steeringVal);
}
