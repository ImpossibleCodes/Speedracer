#include <Servo.h>

static const int steeringPin = 32;
static const int escPin = 26;
// static const int motor = 13;

Servo ESC;
Servo steering;

void setup()
{
  // pinMode(motor, OUTPUT);
  // digitalWrite(motor, HIGH);
  ESC.attach(escPin);
  steering.attach(steeringPin);
  Serial.begin(115200);
  steering.write(100);
}

void loop()
{
  // digitalWrite(motor, HIGH);
  // delay(500);
  // digitalWrite(motor, LOW);
  // delay(500);
  for (int i = 1565; i <= 1565; i++)
  {
    ESC.writeMicroseconds(i);
    Serial.println(i);
    delay(2000);
  }
  delay(750);
  steering.write(130);
  for (int i = 1475; i >= 1475; i--)
  {
    ESC.writeMicroseconds(i);
    Serial.println(i);
    delay(2000);
  }
  delay(750);
  steering.write(70);
}
