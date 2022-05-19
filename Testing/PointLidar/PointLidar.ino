// Updated ESP + Arduino Sample Code
#include "TFMPlus.h" // Include TFMini Plus Library v1.5.0
TFMPlus tfmP_r;        // Create a TFMini Plus object
TFMPlus tfmP_l;        // Create a TFMini Plus object

void setup()
{
    Serial.begin(115200);                                       // Intialize terminal serial port
    delay(20);                                                  // Give port time to initalize
    Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n"); // say 'hello'
    Serial2.begin(115200, SERIAL_8N1, 17, 16);                  // 5, 18 | 17, 16
    delay(20);                                                  // Give port time to initalize
    tfmP_r.begin(&Serial2);                                       // Initialize device library object and...
                                                                // pass device serial port to the object.
    // Send commands to the TFMiniPlus
    // - - Perform a system reset - - - - - - - - - - -
    Serial.println("Soft reset: ");
    if (tfmP_r.sendCommand(SOFT_RESET, 0))
    {
        Serial.println("PASSED\r\n");
    }
    else
        tfmP_r.printReply();

    delay(500); // added to allow the System Rest enough time to complete

    // - - Display the firmware version - - - - - - - - -
    if (tfmP_r.sendCommand(GET_FIRMWARE_VERSION, 0))
    {
        Serial.println("Firmware Version: ");
        Serial.print(tfmP_r.version[0]); // print three single numbers
        Serial.print(".");
        Serial.print(tfmP_r.version[1]);
        Serial.print(".");
        Serial.print(tfmP_r.version[2]);
        Serial.println();
    }
    else
        tfmP_r.printReply();

    // - M- Set the data frame-rate to 5Hz - - - - - - - -
    Serial.println("Data-Frame rate: ");
    if (tfmP_r.sendCommand(SET_FRAME_RATE, FRAME_50))
    {
        Serial.println(FRAME_50);
    }
    else
        tfmP_r.printReply();

    delay(500);

    if (tfmP_r.sendCommand(STANDARD_FORMAT_CM, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_r.printReply();

    // - - - - - - - - - - - - - - - - - - - - - - - -
    delay(500); // And wait for half a second.
}

// Initialize variables
int16_t tfDist = 0; // Distance to object in centimeters
int16_t tfFlux = 0; // Strength or quality of return signal
int16_t tfTemp = 0; // Internal temperature of Lidar sensor chip

void loop()
{
    delay(20); // Loop delay to match the 5Hz data frame rate  - Use: 1/(FRAME_RATE) * 1000

    if (tfmP_r.getData(tfDist, tfFlux, tfTemp))
    {                                      // Get data from the device.
        printf("Dist:%04icm ", tfDist);    // display distance,
        printf("Flux:%05i ", tfFlux);      // display signal strength/quality,
        printf("Temp:%2i%s", tfTemp, "C"); // display temperature,
        printf("\r\n");                    // end-of-line.
    }
    else
    {                      // If the command fails...
        tfmP_r.printFrame(); // display the error and HEX dataa
    }
}
