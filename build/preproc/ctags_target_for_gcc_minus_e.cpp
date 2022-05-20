# 1 "/Users/neilagrawal/Projects/Speedracer/Testing/ESCSteerDemo/ESCSteerDemo.ino"
# 2 "/Users/neilagrawal/Projects/Speedracer/Testing/ESCSteerDemo/ESCSteerDemo.ino" 2

static const int steeringPin = 32;
static const int escPin = 26;
static const int motor = 13;

Servo ESC;
Servo STEER;

void setup()
{
  pinMode(motor, 0x02);
  // digitalWrite(motor, HIGH);
  ESC.attach(escPin);
  STEER.attach(steeringPin);
  Serial.begin(115200);
  STEER.write(100);
}

void loop()
{
  digitalWrite(motor, 0x1);
  delay(500);
  digitalWrite(motor, 0x0);
  delay(500);
  // for (int i = 1565; i <= 1565; i++)
  // {
  //   ESC.writeMicroseconds(i);
  //   Serial.println(i);
  //   delay(2000);
  // }
  // delay(750);
  // STEER.write(130);
  // for (int i = 1475; i >= 1475; i--)
  // {
  //   ESC.writeMicroseconds(i);
  //   Serial.println(i);
  //   delay(2000);
  // }
  // delay(750);
  // STEER.write(70);
}
