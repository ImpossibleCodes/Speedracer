#include <Servo.h>
#include <TFMPlus.h>

static const int steeringPin = 32;
static const int escPin = 26;
static const int FRAMERATE = FRAME_1000;

TFMPlus tfmP_r;
TFMPlus tfmP_l;
TFMPlus tfmP_f;


Servo esc;
Servo steering;

int speedCalc(int distance){
  int temp = distance - 30;
  int speedVal = int(0.000019 * (distance - 30) * (distance - 30) + 1565);
  if (speedVal > 2000)
    return 2000;
  return speedVal;
}

void setup(){

    esc.attach(escPin);
    steering.attach(steeringPin);
    steering.write(100);

    delay(20);                                                  // Give port time to initalize
    Serial1.begin(115200, SERIAL_8N1, 5, 18); //5 , 18 .   22, 23 .   17, 16
    delay(20);
    Serial2.begin(115200,SERIAL_8N1,22,23);
    delay(20);     
    // Give port time to initalize
    Serial.begin(115200, SERIAL_8N1, 17, 16);
    delay(20);
    tfmP_r.begin(&Serial);
    tfmP_l.begin(&Serial1);                                     // Initialize device library object and...
    tfmP_f.begin(&Serial2);

    if (tfmP_r.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_r.printReply();

    delay(500);

    if (tfmP_f.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_f.printReply();

        if (tfmP_l.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_l.printReply();

    delay(500);

    delay(500);
    if (tfmP_r.sendCommand(STANDARD_FORMAT_CM, 0))
    {
    }
    else tfmP_r.printReply();

    delay(500);
    
    if (tfmP_f.sendCommand(STANDARD_FORMAT_CM, 0))
    {
    }
    else tfmP_f.printReply();

        delay(500);
    
    if (tfmP_l.sendCommand(STANDARD_FORMAT_CM, 0))
    {
    }
    else tfmP_l.printReply();

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
bool left = true; //toggle for reading

void loop()
{   
  
    
      if (!tfmP_l.getData(tfDist_l, tfFlux_l, tfTemp_l)){ 
        tfDist_l = 100;   
      }
      if (!tfmP_r.getData(tfDist_r, tfFlux_r, tfTemp_r)){    
                tfDist_r = 100;   

      }
 
    
    if (!tfmP_f.getData(tfDist_f, tfFlux_f, tfTemp_f)){     
      tfDist_f = 100;
    }

    //steering code
    if(tfDist_f > tfDist_l && tfDist_f > tfDist_r){
      
      maxDist = tfDist_f;
      steeringVal = 100;

      if(tfDist_r < 10){
        steeringVal = 90;
      }else if (tfDist_l < 10){
        steeringVal = 110; 
      }
      
    }else if(tfDist_r > tfDist_l){
      
      maxDist = tfDist_r;
      steeringVal = 180;
      
    }else{
       
      maxDist = tfDist_l;
      steeringVal = 70;
        
    }
    int threshold = (int)((0.1379 * speedVal) - 125);
    if(tfDist_f < threshold){

      steeringVal = -steeringVal;
      speedVal = 1380;
     
     }else{
      
      speedVal = speedCalc(maxDist);
             
     }
     
    esc.writeMicroseconds(speedVal);
    steering.write(steeringVal);
    
}


    
