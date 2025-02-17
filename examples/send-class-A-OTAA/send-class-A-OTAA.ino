/**
 * Example of OTAA device      
 * Authors: 
 *        Ivan Moreno
 *        Eduardo Contreras
 *  June 2019
 * 
 * This code is beerware; if you see me (or any other collaborator 
 * member) at the local, and you've found our code helpful, 
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 * 
 * Modified by soldered.com
 */
#include <lorawan.h>

// OTAA Credentials - find yours at https://www.thethingsnetwork.org/ console
// Add your device manually with LoRaWAN 1.0, your area frequency and click on advanced to select class and activation type
// For changing frequency find Config.h file in library source code
const char *devEui = "0000000000000000";
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

const unsigned long interval = 10000; // 10 s interval to send message
unsigned long previousMillis = 0;     // will store last time message sent
unsigned int counter = 0;             // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

// Soldered ESP32 LoRa pins
const sRFM_pins RFM_pins = {
    .CS = 5,
    .RST = 4,
    .DIO0 = 27,
    .DIO1 = 32,
    .DIO2 = 33,
    .DIO5 = -1,
};

void setup()
{
    // Setup loraid access
    Serial.begin(115200);
    while (!Serial)
        ;
    if (!lora.init())
    {
        Serial.println("RFM95 not detected");
        delay(5000);
        return;
    }

    // Set LoRaWAN Class change CLASS_A or CLASS_C
    lora.setDeviceClass(CLASS_A);

    // Set Data Rate
    lora.setDataRate(SF9BW125);

    // set channel to random
    lora.setChannel(MULTI);

    // Put OTAA Key and DevAddress here
    lora.setDevEUI(devEui);
    lora.setAppEUI(appEui);
    lora.setAppKey(appKey);

    // Join procedure
    bool isJoined;
    do
    {
        Serial.println("Joining...");
        isJoined = lora.join();

        //wait for 10s to try again
        delay(10000);
    } while (!isJoined);
    Serial.println("Joined to network");
}

void loop()
{
    // Check interval overflow
    if (millis() - previousMillis > interval)
    {
        previousMillis = millis();

        sprintf(myStr, "Counter-%d", counter);

        Serial.print("Sending: ");
        Serial.println(myStr);

        lora.sendUplink(myStr, strlen(myStr), 0, 1);
        counter++;
    }

    recvStatus = lora.readData(outStr);
    if (recvStatus)
    {
        Serial.println(outStr);
    }

    // Check Lora RX
    lora.update();
}