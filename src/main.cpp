// ******************************* CONFIG **********************************

// Platformio libs
#include <stdint.h>
#include <Arduino.h>

// PIN Configuration
#define MY_RF24_IRQ_PIN 2
#define BUZZER_PIN 3
#define DHT_DATA_PIN 4
#define BUTTON_PIN 5
#define FRONT_PIR_PIN 6
#define MY_RF24_CE_PIN 7
#define MY_RF24_CS_PIN 8
#define LEFT_PIR_PIN 9
#define NEOPIXEL_LED_PIN 10
#define MY_DEFAULT_ERR_LED_PIN 14
#define MY_DEFAULT_TX_LED_PIN 15
#define MY_DEFAULT_RX_LED_PIN 16
#define MY_SIGNING_ATSHA204_PIN 17
#define RIGHT_PIR_PIN 18
#define MQ2_DIGITAL_PIN 19
#define PHOTORES_PIN 20
#define MQ2_ANALOG_PIN 21

// Sampling interval configuration
static const uint64_t UPDATE_INTERVAL = 60000;
static const uint8_t FORCE_UPDATE_N_READS = 10;

// MySensors Configuration
#define MY_REPEATER_FEATURE
#define MY_SIGNING_SOFT
//#define MY_SIGNING_ATSHA204
#define MY_SIGNING_REQUEST_SIGNATURES
#define MY_RADIO_RF24
// Timeout before starting loop without gateway connection
#define MY_TRANSPORT_WAIT_READY_MS 10000
static const bool ack = true;

// DHT22 configuration
#define SENSOR_TEMP_OFFSET 0

// Buzzer frequency
static const uint64_t BUZZER_FREQ = 1500;

// Number of NEOPixel LEDs
#define NUMPIXELS 16
static const uint8_t strip_brightness = 255;

// Debug configuration
//#define MY_DEBUG

// ************************ END OF CONFIG **********************************

// **************************** INIT ***************************************

// Libraries
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <MySensors.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Object initialization
DHT dht;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_LED_PIN, NEO_GRB + NEO_KHZ800);

// CHILD_ID
#define CHILD_ID_FRONT_PIR 1
#define CHILD_ID_LEFT_PIR 2
#define CHILD_ID_RIGHT_PIR 3
#define CHILD_ID_HUM 4
#define CHILD_ID_TEMP 5
#define CHILD_ID_MQ2_D 6
#define CHILD_ID_MQ2_A 7
#define CHILD_ID_BUZZER 8
#define CHILD_ID_RGB_LIGHT 9

// Messages
MyMessage msgFrontPir(CHILD_ID_FRONT_PIR, V_TRIPPED);
MyMessage msgLeftPir(CHILD_ID_LEFT_PIR, V_TRIPPED);
MyMessage msgRightPir(CHILD_ID_RIGHT_PIR, V_TRIPPED);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgGasD(CHILD_ID_MQ2_D, V_STATUS);
MyMessage msgGasA(CHILD_ID_MQ2_A, V_LEVEL);
MyMessage msgBuzzer(CHILD_ID_BUZZER, V_TRIPPED);
MyMessage msgRGB_Dimmer(CHILD_ID_RGB_LIGHT, V_PERCENTAGE);
MyMessage msgRGB_Status(CHILD_ID_RGB_LIGHT, V_STATUS);
MyMessage msgRGB_Color(CHILD_ID_RGB_LIGHT, V_RGB);

// PIRs GLOBAL VARIABLES
bool front_pir = false;
bool left_pir = false;
bool right_pir = false;

// BUZZER GLOBAL VARIABLES
bool buzzer_status = false;
unsigned long buzzer_last_read = 0;

// **************************** END OF INIT ********************************

// ****************************** FUNCTIONS ********************************

// MySensors presentation
void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Repeater", "1.0", ack);
  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_FRONT_PIR, S_MOTION, "Motion sensor", ack);
  present(CHILD_ID_LEFT_PIR, S_MOTION, "Motion sensor", ack);
  present(CHILD_ID_RIGHT_PIR, S_MOTION, "Motion sensor", ack);
  present(CHILD_ID_HUM, S_HUM, "Humidity", ack);
  present(CHILD_ID_TEMP, S_TEMP, "Temperature", ack);
  present(CHILD_ID_MQ2_D, S_BINARY, "Gas alarm", ack);
  present(CHILD_ID_MQ2_A, S_AIR_QUALITY, "Air Quality", ack);
  present(CHILD_ID_BUZZER, S_SOUND, "Buzzer", ack);
  present(CHILD_ID_RGB_LIGHT, S_RGB_LIGHT, "RGB Lamp", ack);
}

// Arduino setup
void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(FRONT_PIR_PIN, INPUT);
  pinMode(LEFT_PIR_PIN, INPUT);
  pinMode(RIGHT_PIR_PIN, INPUT);
  pinMode(MQ2_DIGITAL_PIN, INPUT);
  pinMode(PHOTORES_PIN, INPUT);
  pinMode(MQ2_ANALOG_PIN, INPUT);
  // DHT22 setup
  dht.setup(DHT_DATA_PIN);
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod())
  {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  // Neopixel initializtion
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(strip_brightness);
}

// ************************** END OF FUNCTIONS *****************************

// ******************************* LOOP ************************************
void loop()
{
  // put your main code here, to run repeatedly:
}
// **************************** END OF LOOP *********************************