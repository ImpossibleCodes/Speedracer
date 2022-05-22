//#include <SoftwareSerial.h>

#include <TFMPlus.h> // Include TFMini Plus Library v1.5.0

static const int FRAMERATE = FRAME_100;

TFMPlus tfmP_s;
TFMPlus tfmP_f;

void setup(){

    Serial.begin(115200);                                       // Intialize terminal serial port
    delay(20);                                                  // Give port time to initalize
    Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n"); // say 'hello'
    Serial1.begin(115200, SERIAL_8N1, 5, 18); //5 , 18 .   22, 23 .   17, 16
    delay(20);
    Serial2.begin(115200,SERIAL_8N1,22,23);
    delay(20);                                                  // Give port time to initalize
    tfmP_s.begin(&Serial1);                                     // Initialize device library object and...
    tfmP_f.begin(&Serial2);

    Serial.println("Data-Frame rate: ");
    if (tfmP_s.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_s.printReply();

    delay(500);

        Serial.println("Data-Frame rate: ");
    if (tfmP_f.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_f.printReply();

    delay(500);
    if (tfmP_s.sendCommand(STANDARD_FORMAT_CM, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_s.printReply();

        delay(500);
    if (tfmP_f.sendCommand(STANDARD_FORMAT_CM, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_f.printReply();

    // - - - - - - - - - - - - - - - - - - - - - - - -
    delay(5000); // And wait for 5 seconds.
}

// Initialize variables
int16_t tfDist_s = 0; // Distance to object in centimeters
int16_t tfFlux_s = 0; // Strength or quality of return signal
int16_t tfTemp_s = 0; // Internal temperature of Lidar sensor chip
int16_t tfDist_f = 0; // Distance to object in centimeters
int16_t tfFlux_f = 0; // Strength or quality of return signal
int16_t tfTemp_f = 0; // Internal temperature of Lidar sensor chip
bool right = true;

void loop()
{
    if (!right){

      right = true;
      Serial1.begin(115200, SERIAL_8N1, 5, 18);
      }else{
        
        right = false;
        Serial1.begin(115200, SERIAL_8N1, 17, 16);
        }
    delay(1 / FRAMERATE * 1000 * 3); // Loop delay to match the 5Hz data frame rate  - Use: 1/(FRAME_RATE) * 1000

    if (tfmP_s.getData(tfDist_s, tfFlux_s, tfTemp_s))
    {    
        if(right) {Serial.print("L ");}
        else {Serial.print("R ");}
        Serial.println(tfDist_s);

    }
    else
    {                        // If the command fails...
        tfmP_s.printFrame(); // display the error and HEX data
    }
    
    delay(1 / FRAMERATE * 1000 * 3);
    if (tfmP_f.getData(tfDist_f, tfFlux_f, tfTemp_f))
    {     
        Serial.print("F ");
        Serial.println(tfDist_f);

    }
    else
    {                        // If the command fails...
        tfmP_f.printFrame(); // display the error and HEX data
    }

}
