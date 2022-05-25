# 1 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino"
# 2 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 2
# 3 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 2
# 4 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 2

# 6 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 2

Servo steering;
Servo esc;

rpLidar lidar(&Serial2, 115200, 13, 12);

static void readPoints(void *parameter)
{
  while (true)
  {
    int result = lidar.cacheUltraCapsuledScanData();
    Serial.println(result, 16);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(19, 0x02);
  digitalWrite(19, 0x1);
  esp_task_wdt_init(36000, false); // turn off watchdog so core 0 task doesn't cause reset
  lidar.stopDevice(); // reset the device to be sure that the status is good
  delay(1);
  if (!lidar.start(express))
  {
    Serial.println("failed to start");
    return;
  } // start the express scan of the lidar\  esp_task_wdt_init(36000, false); //turn off watchdog so core 0 task doesn't cause reset
  steering.attach(32);
  esc.attach(26);
  steering.write(100);
  // esc.writeMicroseconds(1565);
  delay(20);
  xTaskCreatePinnedToCore(readPoints, "LidarPolling", 65536, 
# 39 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 3 4
                                                            __null
# 39 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino"
                                                                , 2, 
# 39 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino" 3 4
                                                                     __null
# 39 "/Users/neilagrawal/Projects/Speedracer/360NoseScope/SpeedRacer/SpeedRacer.ino"
                                                                         , 0);
}

void loop()
{
  // grab averages
  float L2SUM = 0.0;
  float L2NUM = 0.0;
  float LSUM = 0.0;
  float LNUM = 0.0;
  float FSUM = 0.0;
  float FNUM = 0.0;
  float RSUM = 0.0;
  float RNUM = 0.0;
  float R2SUM = 0.0;
  float R2NUM = 0.0;
  float THRESHOLD = 800;

  for (int i = 0; i < lidar._cached_scan_node_hq_count; i++)
  {
    scanDot dot;
    dot.angle = (((float)lidar._cached_scan_node_hq_buf[i].angle_z_q14) * 90.0 / 16384.0);
    dot.dist = lidar._cached_scan_node_hq_buf[i].dist_mm_q2 / 4.0f;
    if (dot.angle >= 162.0 and dot.angle <= 198.0) // F
    {
      FSUM += dot.dist;
      FNUM += 1.0;
    }
    else if (dot.angle > 198.0 and dot.angle <= 234.0) // R1
    {
      RSUM += dot.dist;
      RNUM += 1.0;
    }
    else if (dot.angle >= 126.0 and dot.angle < 162.0) // L1
    {
      LSUM += dot.dist;
      LNUM += 1.0;
    }
    else if (dot.angle > 234.0 and dot.angle <= 270.0) // R2
    {
      R2SUM += dot.dist;
      R2NUM += 1.0;
    }
    else if (dot.angle >= 90.0 and dot.angle < 126.0) // L2
    {
      L2SUM += dot.dist;
      L2NUM += 1.0;
    }
  }

  // calculate averages
  float L2 = L2SUM / L2NUM;
  float L = LSUM / LNUM;
  float F = FSUM / FNUM;
  float R = RSUM / RNUM;
  float R2 = R2SUM / R2NUM;

  L = (L + L2) / 2;
  R = (R + R2) / 2;

  // print averages
  // Serial.println(L2);
  Serial.println(L);
  Serial.println(F);
  Serial.println(R);
  // Serial.println(R2);

  // control algorithm
  if (F <= THRESHOLD) // crash imminent
  {
    if (L < THRESHOLD || R < THRESHOLD)
    {
      if (L < R + (THRESHOLD / 4))
      {
        Serial.println("BR");
        steering.write(70);
      }
      else if (R < L + (THRESHOLD / 4))
      {
        Serial.println("BL");
        steering.write(180);
      }
    }
    else
    {
      Serial.println("B");
      steering.write(100);
    }
    esc.writeMicroseconds(1435);
  }
  else
  {
    if (L < THRESHOLD || R < THRESHOLD)
    {
      if (L < R + (THRESHOLD / 4))
      {
        Serial.println("R");
        steering.write(180);
      }
      else if (R < L + (THRESHOLD / 4))
      {
        Serial.println("L");
        steering.write(70);
      }
    }
    else
    {
      Serial.println("F");
      steering.write(100);
    }
    esc.writeMicroseconds(1565);
  }
  delay(10);
}
