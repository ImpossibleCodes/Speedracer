#include <Servo.h>
#include "src/TFMPlus.h" // Include TFMini Plus Library v1.5.0

static const int steeringPin = 32;
static const int escPin = 26;
static const int FRAMERATE = FRAME_100;

Servo ESC;
Servo STEER;

TFMPlus tfmP_l; // Create a TFMini Plus object
TFMPlus tfmP_r; // Create a TFMini Plus object

void setup()
{
    ESC.attach(escPin);
    STEER.attach(steeringPin);
    STEER.write(100);
    Serial.begin(115200);                                       // Intialize terminal serial port
    delay(20);                                                  // Give port time to initalize
    Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n"); // say 'hello'
    Serial1.begin(115200, SERIAL_8N1, 5, 18);                   // 5, 18
    Serial2.begin(115200, SERIAL_8N1, 17, 16);                  // 17, 16
    delay(20);                                                  // Give port time to initalize
    tfmP_l.begin(&Serial1);                                     // Initialize device library object and...
    tfmP_r.begin(&Serial2);                                     // Initialize device library object and...
                                                                // pass device serial port to the object.
    // Send commands to the TFMiniPlus
    // - - Perform a system reset - - - - - - - - - - -
    Serial.println("Soft reset: ");
    if (tfmP_r.sendCommand(SOFT_RESET, 0))
    {
        Serial.println("PASSED\r\n");
    }
    else
    {
        Serial.print("L: ");
        tfmP_l.printReply();
        Serial.print("R: ");
        tfmP_r.printReply();
    }

    delay(500); // added to allow the System Rest enough time to complete

    // - - Display the firmware version - - - - - - - - -
    if (tfmP_r.sendCommand(GET_FIRMWARE_VERSION, 0))
    {
        Serial.println("L Firmware Version: ");
        Serial.print(tfmP_l.version[0]); // print three single numbers
        Serial.print(".");
        Serial.print(tfmP_l.version[1]);
        Serial.print(".");
        Serial.print(tfmP_l.version[2]);
        Serial.println();
        Serial.println("R Firmware Version: ");
        Serial.print(tfmP_r.version[0]); // print three single numbers
        Serial.print(".");
        Serial.print(tfmP_r.version[1]);
        Serial.print(".");
        Serial.print(tfmP_r.version[2]);
        Serial.println();
    }
    else
    {
        tfmP_l.printReply();
        tfmP_r.printReply();
    }

    // - M- Set the data frame-rate to 5Hz - - - - - - - -
    Serial.println("L Data-Frame rate: ");
    if (tfmP_l.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_l.printReply();

    Serial.println("R Data-Frame rate: ");
    if (tfmP_r.sendCommand(SET_FRAME_RATE, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_r.printReply();

    delay(500);

    if (tfmP_l.sendCommand(STANDARD_FORMAT_CM, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_l.printReply();

    if (tfmP_r.sendCommand(STANDARD_FORMAT_CM, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_r.printReply();

    // - - - - - - - - - - - - - - - - - - - - - - - -
    delay(5000); // And wait for half a second.
}

// Initialize variables
int16_t tfDist_l = 0; // Distance to object in centimeters
int16_t tfFlux_l = 0; // Strength or quality of return signal
int16_t tfTemp_l = 0; // Internal temperature of Lidar sensor chip
int16_t tfDist_r = 0; // Distance to object in centimeters
int16_t tfFlux_r = 0; // Strength or quality of return signal
int16_t tfTemp_r = 0; // Internal temperature of Lidar sensor chip

void loop()
{
    delay(1 / FRAMERATE * 1000); // Loop delay to match the 5Hz data frame rate  - Use: 1/(FRAME_RATE) * 1000

    if (tfmP_l.getData(tfDist_l, tfFlux_l, tfTemp_l) && tfmP_r.getData(tfDist_r, tfFlux_r, tfTemp_r))
    {                                                                                                // Get data from the device.
        // Serial.println("L Dist: " + string(tfDist_l) + "L Flux: " + string(tfFlux_l) + "L Temp: " + string(tfTemp_l) + "C"); // left display distance, signal strength/quality, and temperature
        // Serial.println("R Dist: " + string(tfDist_r) + "R Flux: " + string(tfFlux_r) + "R Temp: " + string(tfTemp_r) + "C"); // right display distance, signal strength/quality, and temperature                       // end-of-line.

        if (tfDist_l > tfDist_r)
        {
            // Serial.println("L");
            STEER.write(70);
            ESC.writeMicroseconds(1565);
        }
        else if (tfDist_r > tfDist_l)
        {
            // Serial.println("R");
            STEER.write(130);
            ESC.writeMicroseconds(1565);
        }
        else
        {
            // Serial.println("N");
            STEER.write(100);
            ESC.writeMicroseconds(1565);
        }
        Serial.println("");
    }
    else
    {                        // If the command fails...
        tfmP_l.printFrame(); // display the error and HEX data
        tfmP_r.printFrame(); // display the error and HEX data
    }
}
