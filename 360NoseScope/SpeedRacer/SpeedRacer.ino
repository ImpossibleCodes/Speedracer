#include "rpLidar.h"
#include "rpLidarTypes.h"
#include <esp_task_wdt.h>

#include <Servo.h>

Servo myservo;
Servo esc;

rpLidar lidar(&Serial2, 115200, 13, 12);

static void readPoints(void * parameter) {
  while (true) {
    int result = lidar.cacheUltraCapsuledScanData();
    Serial.println(result, HEX);
  }
}
float average(int * pointCloud, int len) {
  long sum = 0L;
  for (int i = 0 ; i < len; i++)
    sum += pointCloud[i];
  return  ((float) sum) / len ;
}
void setup() {

  Serial.begin(115200);
  pinMode(19, OUTPUT);
  digitalWrite(19, HIGH);
  esp_task_wdt_init(36000, false); //turn off watchdog so core 0 task doesn't cause reset
  lidar.stopDevice(); //reset the device to be sure that the status is good
  delay(1);
  if (!lidar.start(express)) {
    Serial.println("failed to start");
    return;
  } //start the express scan of the lidar\  esp_task_wdt_init(36000, false); //turn off watchdog so core 0 task doesn't cause reset
  myservo.attach(32);
  esc.attach(26);
  myservo.write(100);
  esc.writeMicroseconds(1565);
  delay(20);
  xTaskCreatePinnedToCore(readPoints, "LidarPolling", 65536, NULL, 2, NULL, 0);
}
float LSUM = 0.0;
float LNUM = 0.0;
float RSUM = 0.0;
float RNUM = 0.0;
float THRESHOLD = 1100;
void loop()
{
  for (int i = 0; i < lidar._cached_scan_node_hq_count; i++) {
    scanDot dot;
    dot.angle = (((float)lidar._cached_scan_node_hq_buf[i].angle_z_q14) * 90.0 / 16384.0);
    dot.dist = lidar._cached_scan_node_hq_buf[i].dist_mm_q2 / 4.0f;
    if (dot.angle >= 180.0 and dot.angle <= 210) {
      LSUM += dot.dist;
      LNUM += 1.0;
    }
    if (dot.angle >= 120.0 and dot.angle <= 180.0) {
      RSUM += dot.dist;
      RNUM += 1.0;
    }

  }
  Serial.println(LSUM / LNUM);
  Serial.println(RSUM / RNUM);
  if (LSUM / LNUM < THRESHOLD or RSUM / RNUM < THRESHOLD) {
    if (LSUM / LNUM < RSUM / RNUM) {
      Serial.println("L");
      myservo.write(70);
    }
    else {
      Serial.println("R");
      myservo.write(180);
    }

  }
  else {
    Serial.println("S");
    myservo.write(100);
  }
  esc.writeMicroseconds(1565);
  delay(10);
  LSUM = 0.0;
  LNUM = 0.0;
  RSUM = 0.0;
  RNUM = 0.0;


}
