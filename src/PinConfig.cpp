/* d-diot nano repeater - PIN configuration

 On the basis of the devices attached to your Arduino nano, comment or uncomment the following lines.
 Change the PIN number according to your needs. 
 PIN 2-13  = D2-D13
 PIN 14-21 = A0-A7 
 
 The following pin are not available and defined in MySConfig.cpp
 IRQ PIN = D2
 CE PIN = D7
 CS PIN = D8

The signaling LED pin are defined in MySConfigg.cpp; comment the lines out if you need the pins for other scope
ERROR LED = A0
TX LED = A1
RX LED = A2

 Uncomment and define the MY_SIGNING_ATSHA204_PIN line if you want to use an ATSHA204 IC for signing.
*/

#define BUZZER_PIN 3
//#define DHT_DATA_PIN 4
//#define RGBW_R_PIN 5
//#define FRONT_PIR_PIN 5
//#define RGBW_G_PIN 6
//#define LEFT_PIR_PIN 6
//#define RGBW_B_PIN 9
//#define RIGHT_PIR_PIN 9
//#define RGBW_W_PIN 10
//#define MY_SIGNING_ATSHA204_PIN 17
#define BUTTON_PIN 17
#define NEOPIXEL_LED_PIN 18
#define MQ2_DIGITAL_PIN 19
//#define PHOTORES_PIN 20
#define MQ2_ANALOG_PIN 21