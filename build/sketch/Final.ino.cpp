#include <Arduino.h>
#line 1 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
#include <Servo.h>
#include <esp_task_wdt.h>
#include "src/rpLidar.h"
#include "src/rpLidarTypes.h"

static const int steeringPin = 32;
static const int escPin = 26;

Servo esc;
Servo steering;

rpLidar lidar(&Serial1, 115200, 13, 12);

#line 14 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
static void readPoints(void *parameter);
#line 23 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
float average(int *pointCloud, int len);
#line 31 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
void setup();
#line 54 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
void loop();
#line 14 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
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
    pinMode(19, OUTPUT);
    digitalWrite(19, HIGH);
    esc.attach(escPin);
    steering.attach(steeringPin);
    steering.write(100);
    delay(20);
    xTaskCreatePinnedToCore(readPoints, "LidarPolling", 65536, NULL, 2, NULL, 0);
}

int threshold = 15; // cm

void loop()
{
    int farthest_point_angle = lidar.getMeasurePoints(lidar._cached_scan_node_hq_count); // get the lidar data
    // lidar_buffer[angle] = distance

    // int farthest_point_angle = lidar_data;
    // double farthest_point_angle_distance = get<1>(lidar_data);

    // for (int i = -80; i <= 80; i++)
    // {
    //     if (lidar_buffer[(i + 360) % 360] > farthest_point_angle_distance)
    //     {
    //         farthest_point_angle_distance = lidar_buffer[(i + 360) % 360];
    //         farthest_point_angle = i;
    //         Serial.println(i);
    //     }
    // }

    // if (160 <= farthest_point_angle && farthest_point_angle <= 200) // straight
    // {
    //     steer.write(100);
    // }
    // else if (farthest_point_angle < 160) // turn left
    // {
    //     steer.write(farthest_point_angle - 80);
    // }
    // else if (200 < farthest_point_angle) // turn right
    // {
    //     steer.write(farthest_point_angle - 80);
    // }
    Serial.println(farthest_point_angle);
    steering.write(farthest_point_angle + 100);
    esc.writeMicroseconds(1570);
}

