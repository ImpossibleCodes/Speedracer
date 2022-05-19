#include "rpLidar.h"
#include "rpLidarTypes.h"
#include <esp_task_wdt.h>
#include <Servo.h>

Servo myservo;

rpLidar lidar(&Serial2, 115200, 13, 12);

static void readPoints(void *parameter)
{
  while (true)
  {
    int result = lidar.cacheUltraCapsuledScanData();
    Serial.println(result, HEX);
  }
}
float average(int *pointCloud, int len)
{
  long sum = 0L;
  for (int i = 0; i < len; i++)
    sum += pointCloud[i];
  return ((float)sum) / len;
}
void setup()
{

  Serial.begin(115200);
  esp_task_wdt_init(36000, false); // turn off watchdog so core 0 task doesn't cause reset
  lidar.stopDevice();              // reset the device to be sure that the status is good
  delay(1);
  if (!lidar.start(express))
  {
    Serial.println("failed to start");
    return;
  } // start the express scan of the lidar\  esp_task_wdt_init(36000, false); //turn off watchdog so core 0 task doesn't cause reset
  myservo.attach(2);
  myservo.write(40);
  delay(20);
  xTaskCreatePinnedToCore(readPoints, "LidarPolling", 65536, NULL, 2, NULL, 0);
}
double *lidar_buffer;
int threshold = 500;

void loop()
{

  lidar_buffer = lidar.DebugPrintMeasurePoints(lidar._cached_scan_node_hq_count);
  String direction1 = String(Serial.read());
  Serial.println(direction1);
  if (direction1 == "L")
  {
    myservo.write(50);
  }
  if (direction1 == "R")
  {
    myservo.write(150);
  }
  else
  {
    myservo.write(90);
  }
  delay(20);
}
