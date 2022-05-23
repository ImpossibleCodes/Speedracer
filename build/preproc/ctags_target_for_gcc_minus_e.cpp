# 1 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
# 2 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 2
# 3 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 2
# 4 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 2
# 5 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 2

static const int steeringPin = 32;
static const int escPin = 26;

Servo esc;
Servo steering;

rpLidar lidar(&Serial1, 115200, 13, 12);

static void readPoints(void *parameter)
{
    while (true)
    {
        int result = lidar.cacheUltraCapsuledScanData();
        Serial.println(result, 16);
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
    lidar.stopDevice(); // reset the device to be sure that the status is good
    delay(1);
    if (!lidar.start(express))
    {
        Serial.println("failed to start");
        return;
    } // start the express scan of the lidar\  esp_task_wdt_init(36000, false); //turn off watchdog so core 0 task doesn't cause reset
    pinMode(19, 0x02);
    digitalWrite(19, 0x1);
    esc.attach(escPin);
    steering.attach(steeringPin);
    steering.write(100);
    delay(20);
    xTaskCreatePinnedToCore(readPoints, "LidarPolling", 65536, 
# 49 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 3 4
                                                              __null
# 49 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
                                                                  , 2, 
# 49 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino" 3 4
                                                                       __null
# 49 "/Users/neilagrawal/Projects/Speedracer/Final/Final.ino"
                                                                           , 0);
}

int threshold = 15; // cm

void loop()
{
    double *point = lidar.getMeasurePoints(lidar._cached_scan_node_hq_count); // get the lidar data
    int farthest_point_angle = (int) point[0];
    double farthest_point_distance = point[1];
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
    if (farthest_point_distance < threshold && -20 <= farthest_point_angle && farthest_point_angle <= 20)
    {
        Serial.println("Detected Imminent Collision | " + (String) (farthest_point_angle) + " " + (String) (farthest_point_distance));
        steering.write(farthest_point_angle + 100);
        esc.writeMicroseconds(1570);
    }
    else
    {
        Serial.println(String(farthest_point_angle) + " " + String(farthest_point_distance));
        steering.write(farthest_point_angle + 100);
        esc.writeMicroseconds(1570);
    }
}
