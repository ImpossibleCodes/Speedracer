#include <Servo.h>

static const int steeringPin = 32;
static const int escPin = 26;

Servo ESC;
Servo STEER;

void setup()
{
  ESC.attach(escPin);
  STEER.attach(steeringPin);
  Serial.begin(115200);
  STEER.write(100);
}

void loop()
{
  for (int i = 1565; i <= 1565; i++)
  {
    ESC.writeMicroseconds(i);
    Serial.println(i);
    delay(2000);
  }
  delay(750);
  STEER.write(130);
  for (int i = 1475; i >= 1475; i--)
  {
    ESC.writeMicroseconds(i);
    Serial.println(i);
    delay(2000);
  }
  delay(750);
  STEER.write(70);
}
