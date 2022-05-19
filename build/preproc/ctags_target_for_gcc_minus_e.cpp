# 1 "/Users/neilagrawal/Projects/Speedracer/Testing/PointLidar/PointLidar.ino"
// Updated ESP + Arduino Sample Code
# 3 "/Users/neilagrawal/Projects/Speedracer/Testing/PointLidar/PointLidar.ino" 2
TFMPlus tfmP; // Create a TFMini Plus object

void setup()
{
    Serial.begin(115200); // Intialize terminal serial port
    delay(20); // Give port time to initalize
    Serial.println("\r\nTFMPlus Library Example w/ ESP32\r\n"); // say 'hello'
    Serial2.begin(115200, 0x800001c, 17, 16); // 5, 18 | 17, 16
    delay(20); // Give port time to initalize
    tfmP.begin(&Serial2); // Initialize device library object and...
                                                                // pass device serial port to the object.
    // Send commands to the TFMiniPlus
    // - - Perform a system reset - - - - - - - - - - -
    Serial.println("Soft reset: ");
    if (tfmP.sendCommand(0x00020405 /* returns a 1 byte pass/fail (0/1)*/, 0))
    {
        Serial.println("PASSED\r\n");
    }
    else
        tfmP.printReply();

    delay(500); // added to allow the System Rest enough time to complete

    // - - Display the firmware version - - - - - - - - -
    if (tfmP.sendCommand(0x00010407 /* returns 3 byte firmware version*/, 0))
    {
        Serial.println("Firmware Version: ");
        Serial.print(tfmP.version[0]); // print three single numbers
        Serial.print(".");
        Serial.print(tfmP.version[1]);
        Serial.print(".");
        Serial.print(tfmP.version[2]);
        Serial.println();
    }
    else
        tfmP.printReply();

    // - M- Set the data frame-rate to 5Hz - - - - - - - -
    Serial.println("Data-Frame rate: ");
    if (tfmP.sendCommand(0x00030606 /* These commands each return*/, 0x0032))
    {
        Serial.println(0x0032);
    }
    else
        tfmP.printReply();

    delay(500);

    if (tfmP.sendCommand(0x01050505 /*           "*/, 0))
    {
        Serial.println("CM");
    }
    else
        tfmP.printReply();

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

    if (tfmP.getData(tfDist, tfFlux, tfTemp))
    { // Get data from the device.
        printf("Dist:%04icm ", tfDist); // display distance,
        printf("Flux:%05i ", tfFlux); // display signal strength/quality,
        printf("Temp:%2i%s", tfTemp, "C"); // display temperature,
        printf("\r\n"); // end-of-line.
    }
    else
    { // If the command fails...
        tfmP.printFrame(); // display the error and HEX dataa
    }
}
