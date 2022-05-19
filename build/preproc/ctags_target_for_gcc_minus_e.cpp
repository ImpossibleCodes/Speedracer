# 1 "/Users/neilagrawal/Projects/Speedracer/Testing/PointLidar/PointLidar.ino"
// Updated ESP + Arduino Sample Code
# 3 "/Users/neilagrawal/Projects/Speedracer/Testing/PointLidar/PointLidar.ino" 2
TFMPlus tfmP_l; // Create a TFMini Plus object
TFMPlus tfmP_r; // Create a TFMini Plus object

static const int FRAMERATE = 0x0032;

void setup()
{
    Serial.begin(115200); // Intialize terminal serial port
    delay(20); // Give port time to initalize
    Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n"); // say 'hello'
    Serial1.begin(115200, 0x800001c, 5, 18); // 5, 18
    Serial2.begin(115200, 0x800001c, 17, 16); // 17, 16
    delay(20); // Give port time to initalize
    tfmP_l.begin(&Serial1); // Initialize device library object and...
    tfmP_r.begin(&Serial2); // Initialize device library object and...
                                                                // pass device serial port to the object.
    // Send commands to the TFMiniPlus
    // - - Perform a system reset - - - - - - - - - - -
    Serial.println("Soft reset: ");
    if (tfmP_r.sendCommand(0x00020405 /* returns a 1 byte pass/fail (0/1)*/, 0))
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
    if (tfmP_r.sendCommand(0x00010407 /* returns 3 byte firmware version*/, 0))
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
    if (tfmP_l.sendCommand(0x00030606 /* These commands each return*/, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_l.printReply();

    Serial.println("R Data-Frame rate: ");
    if (tfmP_r.sendCommand(0x00030606 /* These commands each return*/, FRAMERATE))
    {
        Serial.println(FRAMERATE);
    }
    else
        tfmP_r.printReply();

    delay(500);

    if (tfmP_l.sendCommand(0x01050505 /*           "*/, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_l.printReply();

    if (tfmP_r.sendCommand(0x01050505 /*           "*/, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP_r.printReply();

    // - - - - - - - - - - - - - - - - - - - - - - - -
    delay(500); // And wait for half a second.
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
    { // Get data from the device.
        printf("L Dist:%04icm ", tfDist_l); // display distance,
        printf("L Flux:%05i ", tfFlux_l); // display signal strength/quality,
        printf("L Temp:%2i%s", tfTemp_l, "C"); // display temperature,
        printf("\r"); // end-of-line.
        printf("R Dist:%04icm ", tfDist_r); // display distance,
        printf("R Flux:%05i ", tfFlux_r); // display signal strength/quality,
        printf("R Temp:%2i%s", tfTemp_r, "C"); // display temperature,
        printf("\r\n"); // end-of-line.
    }
    else
    { // If the command fails...
        tfmP_l.printFrame(); // display the error and HEX data
        tfmP_r.printFrame(); // display the error and HEX data
    }
}
