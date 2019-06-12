// ******************************* CONFIG **********************************

// Platformio libs
#include <stdint.h>
#include <Arduino.h>

// PIN Configuration
#define MY_RF24_IRQ_PIN 2
#define BUZZER_PIN 3
//#define DHT_DATA_PIN 4
//#define RGBW_R_PIN 5
//#define FRONT_PIR_PIN 5
//#define RGBW_G_PIN 6
//#define LEFT_PIR_PIN 6
#define MY_RF24_CE_PIN 7
#define MY_RF24_CS_PIN 8
//#define RGBW_B_PIN 9
//#define RIGHT_PIR_PIN 9
//#define RGBW_W_PIN 10
#define MY_DEFAULT_ERR_LED_PIN 14
#define MY_DEFAULT_TX_LED_PIN 15
#define MY_DEFAULT_RX_LED_PIN 16
//#define MY_SIGNING_ATSHA204_PIN 17
//#define BUTTON_PIN 17
#define NEOPIXEL_LED_PIN 18
#define MQ2_DIGITAL_PIN 19
//#define PHOTORES_PIN 20
#define MQ2_ANALOG_PIN 21

// Sampling interval configuration
static const uint64_t UPDATE_INTERVAL = 60000;
static const uint8_t FORCE_UPDATE_N_READS = 10;

// Save states to EEPROM (useful but the total write cycle are limited to about 100000)
#define SAVE_STATE_TO_EEPROM

// DHT22 configuration
#ifdef DHT_DATA_PIN
#define SENSOR_TEMP_OFFSET 0
#endif

// Buzzer configuration
#ifdef BUZZER_PIN
#define BUZZER_FREQ 500
#endif

// NEOPixel LEDs configuration
#ifdef NEOPIXEL_LED_PIN
#define NUMPIXELS 16
#define STRIP_BRIGHTNESS 255
#endif

// MQ2 configuration
#ifdef MQ2_DIGITAL_PIN
#ifdef BUZZER_PIN
#define SOUND_ALARM
#ifdef SOUND_ALARM
#define BUZZER_ALARM_FREQ 1500
#define ON_PERIOD 500
#define OFF_PERIOD 500
#endif
#endif
#ifdef NEOPIXEL_LED_PIN
#define VISUAL_ALARM
#ifdef VISUAL_ALARM
#define LIGHT_ON_PERIOD 1000
#define LIGHT_OFF_PERIOD 1000
#define LIGHT_RED_VALUE 255
#define LIGHT_GREEN_VALUE 0
#define LIGHT_BLU_VALUE 0
#define LIGHT_DIMMER_PERCENT 100
#endif
#endif
#endif

// MySensors Configuration
#define MY_REPEATER_FEATURE
#define MY_SIGNING_SOFT
#define MY_SIGNING_REQUEST_SIGNATURES
#ifdef MY_SIGNING_ATSHA204_PIN
#define MY_SIGNING_ATSHA204
#endif
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX
// Timeout before starting loop without gateway connection
#define MY_TRANSPORT_WAIT_READY_MS 10000
#define MY_SPLASH_SCREEN_DISABLED
static const bool ack = false;

// Debug configuration
//#define MY_DEBUG

// ************************ END OF CONFIG **********************************

// **************************** INIT ***************************************

// Libraries
#ifdef DHT_DATA_PIN
#include <DHT.h>
#endif
#ifdef NEOPIXEL_LED_PIN
#include <Adafruit_NeoPixel.h>
#endif
#include <MySensors.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Object initialization
#ifdef DHT_DATA_PIN
DHT dht;
#endif
#ifdef NEOPIXEL_LED_PIN
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

// CHILD_ID
#ifdef FRONT_PIR_PIN
#define CHILD_ID_FRONT_PIR 1
#endif
#ifdef LEFT_PIR_PIN
#define CHILD_ID_LEFT_PIR 2
#endif
#ifdef RIGHT_PIR_PIN
#define CHILD_ID_RIGHT_PIR 3
#endif
#ifdef DHT_DATA_PIN
#define CHILD_ID_HUM 4
#define CHILD_ID_TEMP 5
#endif
#ifdef MQ2_DIGITAL_PIN
#define CHILD_ID_MQ2_D 6
#endif
#ifdef MQ2_ANALOG_PIN
#define CHILD_ID_MQ2_A 7
#endif
#ifdef BUZZER_PIN
#define CHILD_ID_BUZZER 8
#endif
#ifdef NEOPIXEL_LED_PIN
#define CHILD_ID_RGB_LIGHT 9
#endif
#ifdef RGBW_R_PIN
#ifdef RGBW_G_PIN
#ifdef RGBW_B_PIN
#ifdef RGBW_W_PIN
#define CHILD_ID_RGBW_LIGHT 10
#endif
#endif
#endif
#endif
#ifdef PHOTORES_PIN
#define CHILD_ID_LIGHT_LEVEL 11
#endif
#ifdef SOUND_ALARM
#define CHILD_ID_ENABLE_SOUND_ALARM 12
#endif
#ifdef VISUAL_ALARM
#define CHILD_ID_ENABLE_VISUAL_ALARM 13
#endif

// Messages
#ifdef CHILD_ID_FRONT_PIR
MyMessage msgFrontPir(CHILD_ID_FRONT_PIR, V_TRIPPED);
#endif
#ifdef CHILD_ID_LEFT_PIR
MyMessage msgLeftPir(CHILD_ID_LEFT_PIR, V_TRIPPED);
#endif
#ifdef CHILD_ID_RIGHT_PIR
MyMessage msgRightPir(CHILD_ID_RIGHT_PIR, V_TRIPPED);
#endif
#ifdef CHILD_ID_HUM
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
#endif
#ifdef CHILD_ID_TEMP
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
#endif
#ifdef CHILD_ID_MQ2_D
MyMessage msgGasD(CHILD_ID_MQ2_D, V_TRIPPED);
#endif
#ifdef CHILD_ID_MQ2_A
MyMessage msgGasA(CHILD_ID_MQ2_A, V_LEVEL);
#endif
#ifdef CHILD_ID_BUZZER
MyMessage msgBuzzer(CHILD_ID_BUZZER, V_ARMED);
#endif
#ifdef CHILD_ID_RGB_LIGHT
MyMessage msgRGB_Dimmer(CHILD_ID_RGB_LIGHT, V_PERCENTAGE);
MyMessage msgRGB_Status(CHILD_ID_RGB_LIGHT, V_STATUS);
MyMessage msgRGB_Color(CHILD_ID_RGB_LIGHT, V_RGB);
#endif
#ifdef CHILD_ID_RGBW_LIGHT
MyMessage msgRGBW_Dimmer(CHILD_ID_RGBW_LIGHT, V_PERCENTAGE);
MyMessage msgRGBW_Status(CHILD_ID_RGBW_LIGHT, V_STATUS);
MyMessage msgRGBW_Color(CHILD_ID_RGBW_LIGHT, V_RGBW);
#endif
#ifdef CHILD_ID_LIGHT_LEVEL
MyMessage msgLightLevel(CHILD_ID_LIGHT_LEVEL, V_LIGHT_LEVEL);
#endif
#ifdef CHILD_ID_ENABLE_SOUND_ALARM
MyMessage msgSoundAlarm(CHILD_ID_ENABLE_SOUND_ALARM, V_STATUS);
#endif
#ifdef CHILD_ID_ENABLE_VISUAL_ALARM
MyMessage msgVisualAlarm(CHILD_ID_ENABLE_VISUAL_ALARM, V_STATUS);
#endif

// PIRs STATUS VARIABLES
#ifdef FRONT_PIR_PIN
bool front_pir = false;
#endif
#ifdef LEFT_PIR_PIN
bool left_pir = false;
#endif
#ifdef RIGHT_PIR_PIN
bool right_pir = false;
#endif

// BUZZER GLOBAL VARIABLES
#ifdef BUZZER_PIN
bool buzzer_status = false;
bool buzzer_set_status = false;
unsigned long buzzer_last_start = 0;
#endif

// MQ2 GLOBAL VARIABLES
#ifdef MQ2_DIGITAL_PIN
bool mq2_d_alarm = true;
#ifdef SOUND_ALARM
bool enable_sound_alarm = false;
unsigned long sound_start_time = 0;
bool mq2_trigger_sound_alarm = false;
#endif
#ifdef VISUAL_ALARM
bool enable_visual_alarm = false;
bool mq2_trigger_visual_alarm = false;
unsigned long lamp_on_start_time = 0;
uint8_t prealarm_R_value;
uint8_t prealarm_G_value;
uint8_t prealarm_B_value;
bool prealarm_lamp_status;
uint8_t prealarm_dimmer_percentage;
#endif
#ifdef MQ2_ANALOG_PIN
int lastGasValue;
uint8_t nNoUpdatesGas;
#endif
#endif

// DHT22 GLOBAL VARIABLES
#ifdef DHT_DATA_PIN
float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;
bool metric = true;
#endif

// PHOTORESISTOR GLOBAL VARIABLES
#ifdef PHOTORES_PIN
int lastLightLevel;
uint8_t nNoUpdatesLightLevel;
#endif

// RGB LAMP GLOBAL VARIABLES
#ifdef NEOPIXEL_LED_PIN
long set_RGB_values[3] = {255, 255, 255};
bool set_rgb_lamp_status = false;
uint8_t set_rgb_lamp_dimmer_percent = 0;
uint8_t last_R_value = 255;
uint8_t last_G_value = 255;
uint8_t last_B_value = 255;
bool last_rgb_lamp_status = false;
uint8_t last_rgb_lamp_dimmer_percent = 0;
bool trigger_show_strip = true;
char rgb_char[7];
#endif

// UPDATE INTERVAL
unsigned long last_update_time = 0;
unsigned long current_time = 0;
bool is_first_run = true;

// ********************************* END OF INIT **********************************

// ****************************** CUSTOM FUNCTIONS ********************************

// Mean and smooth the analog reads
int analog_smooth(int PIN, int reads)
{
  int smoothed = 0;
  for (int i = 0; i < reads; i++)
  {
    smoothed = (smoothed * i - 1 + analogRead(PIN)) / (i + 1);
  }
  return smoothed;
}

// Light Level read and update
#ifdef CHILD_ID_LIGHT_LEVEL
void update_light_level(int tol = 2, int reads = 10)
{
  int value = map(analog_smooth(PHOTORES_PIN, reads), 0, 1023, 0, 100);
#ifdef MY_DEBUG
  Serial.print("Light Level: ");
  Serial.println(value);
#endif
  if (value > lastLightLevel + tol || value < lastLightLevel - tol || nNoUpdatesLightLevel == FORCE_UPDATE_N_READS)
  {
    nNoUpdatesLightLevel = 0;
    lastLightLevel = value;
    send(msgLightLevel.set(lastLightLevel), ack);
  }
  else
  {
    nNoUpdatesLightLevel++;
  }
}
#endif

// MQ2 analog Level read and update
#ifdef CHILD_ID_MQ2_A
void update_gas_level(int tol = 2, int reads = 10)
{
  int value = map(analog_smooth(MQ2_ANALOG_PIN, reads), 0, 1023, 0, 100);
#ifdef MY_DEBUG
  Serial.print("Gas Level: ");
  Serial.println(value);
#endif
  if (value > lastGasValue + tol || value < lastGasValue - tol || nNoUpdatesGas == FORCE_UPDATE_N_READS)
  {
    nNoUpdatesGas = 0;
    lastGasValue = value;
    send(msgGasA.set(lastGasValue), ack);
  }
  else
  {
    lastGasValue++;
  }
}
#endif

// DHT 22 read and update
#ifdef DHT_DATA_PIN
void update_temp_and_hum()
{
  dht.readSensor(true);
  // Get temperature from DHT library
  float temperature = dht.getTemperature();
#ifdef MY_DEBUG
  Serial.print("Temp: ");
  Serial.println(temperature);
#endif
  if (isnan(temperature))
  {
    Serial.println("Failed reading temperature from DHT!");
  }
  else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS)
  {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp = temperature;
    // apply the offset before converting to something different than Celsius degrees
    temperature += SENSOR_TEMP_OFFSET;

    if (!metric)
    {
      temperature = dht.toFahrenheit(temperature);
    }
    // Reset no updates counter
    nNoUpdatesTemp = 0;
    send(msgTemp.set(temperature, 1), ack);
  }
  else
  {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp++;
  }
  // Get humidity from DHT library
  float humidity = dht.getHumidity();
#ifdef MY_DEBUG
  Serial.print("Hum: ");
  Serial.println(humidity);
#endif
  if (isnan(humidity))
  {
    Serial.println("Failed reading humidity from DHT");
  }
  else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS)
  {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
    // Reset no updates counter
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1), ack);
  }
  else
  {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum++;
  }
}
#endif

// Buzzer update
#ifdef CHILD_ID_BUZZER
void buzzer_update(bool is_mq2_alarm = false)
{
  if (!is_mq2_alarm)
  {
    buzzer_status = buzzer_set_status;
    // Activate or deactivate Buzzer
    (buzzer_status ? tone(BUZZER_PIN, BUZZER_FREQ) : noTone(BUZZER_PIN));
    // Send feedback
    send(msgBuzzer.set(buzzer_status ? 1 : 0), ack);
  }
#ifdef SOUND_ALARM
  if (is_mq2_alarm)
  {
    // Logic is reversed False or 0 = GAS = DANGER!
    if (!mq2_d_alarm)
    {
      buzzer_set_status = true;
      buzzer_status = true;
      // Send feedback
      send(msgBuzzer.set(buzzer_status ? 1 : 0), ack);
    }
    // Disable buzzer when the GAS alarm is OFF
    else
    {
      buzzer_set_status = false;
      buzzer_status = false;
      noTone(BUZZER_PIN);
      // Send feedback
      send(msgBuzzer.set(buzzer_status ? 1 : 0), ack);
    }
  }
#endif
#ifdef MY_DEBUG
  Serial.print("Buzzer status: ");
  Serial.println(buzzer_status);
#endif
}
#endif

// Buzzer pulse
#ifdef SOUND_ALARM
void buzzer_pulse(uint64_t freq = BUZZER_ALARM_FREQ, uint64_t on_time = ON_PERIOD, uint64_t off_time = OFF_PERIOD)
{
  // Pulsed sound
  if (sound_start_time == 0)
  {
    tone(BUZZER_PIN, freq, on_time);
    sound_start_time = current_time;
  }
  else if (current_time > sound_start_time + on_time + off_time)
  {
    sound_start_time = 0;
  }
}
#endif

// uint8_t to HEX char (4 bit)
char nibble_to_hex(uint8_t nibble)
{ // convert a 4-bit nibble to a hexadecimal character
  nibble &= 0xF;
  return nibble > 9 ? nibble - 10 + 'A' : nibble + '0';
}

// Convert LAMP RGB values to hex
#ifdef CHILD_ID_RGB_LIGHT
void rgb_to_hex_array(uint8_t R = last_R_value, uint8_t G = last_G_value, uint8_t B = last_B_value)
{
  rgb_char[0] = nibble_to_hex(R >> 4);
  rgb_char[1] = nibble_to_hex(R);
  rgb_char[2] = nibble_to_hex(G >> 4);
  rgb_char[3] = nibble_to_hex(G);
  rgb_char[4] = nibble_to_hex(B >> 4);
  rgb_char[5] = nibble_to_hex(B);
}
#endif

// ************************** END OF CUSTOM FUNCTIONS *****************************/

// ************************ MYSESNSORS RECEIVE FUNCTION ***************************/

void receive(const MyMessage &message)
{
  // Do nothing if message is ACK
  if (message.isAck())
  {
    return;
  }
  // Process the messages sent to the Buzzer (CHILD_ID_BUZZER)
#ifdef CHILD_ID_BUZZER
  if (message.sensor == CHILD_ID_BUZZER)
  {
    // Extract and process ARMED values
    if (message.type == V_ARMED)
    {
#ifdef MY_DEBUG
      Serial.print("Received Buzzer V_ARMED Message: ");
      Serial.println(message.getInt());
#endif
// Ignore messages when MQ2 sound alarm is active. Logic is reverse false or 0 = GAS = DANGER!
#ifdef SOUND_ALARM
      if (enable_sound_alarm && !mq2_d_alarm)
      {
        return;
      }
#endif
      buzzer_set_status = (message.getInt() ? true : false);
    }
  }
#endif
  // Process the messages sent from the Enable sound alarm switch (CHILD_ID_ENABLE_SOUND_ALARM)
#ifdef CHILD_ID_ENABLE_SOUND_ALARM
  if (message.sensor == CHILD_ID_ENABLE_SOUND_ALARM)
  {
    // Extract and process STATUS values
    if (message.type == V_STATUS)
    {
      enable_sound_alarm = (message.getInt() ? true : false);
#ifdef MY_DEBUG
      Serial.print("Received enable sound alarm V_STATUS Message: ");
      Serial.println(message.getInt());
#endif
      // send feedback
      send(msgSoundAlarm.set(enable_sound_alarm ? 1 : 0), ack);
// Save to eeprom position 0
#ifdef SAVE_STATE_TO_EEPROM
      saveState(0, enable_sound_alarm ? 1 : 0);
#endif
    }
  }
#endif
// Process the messages sent from the Enable visual alarm switch (CHILD_ID_ENABLE_VISUAL_ALARM)
#ifdef CHILD_ID_ENABLE_VISUAL_ALARM
  if (message.sensor == CHILD_ID_ENABLE_VISUAL_ALARM)
  {
    // Extract and process STATUS values
    if (message.type == V_STATUS)
    {
      enable_visual_alarm = (message.getInt() ? true : false);
#ifdef MY_DEBUG
      Serial.print("Received enable visual alarm V_STATUS Message: ");
      Serial.println(message.getInt());
#endif
      // send feedback
      send(msgVisualAlarm.set(enable_visual_alarm ? 1 : 0), ack);
      // Save to eeprom position 1
#ifdef SAVE_STATE_TO_EEPROM
      saveState(1, enable_visual_alarm ? 1 : 0);
#endif
    }
  }
#endif
  // Process the messages sent to RGB LAMP (CHILD ID = CHILD_ID_RGB_LIGHT)
#ifdef CHILD_ID_RGB_LIGHT
  if (message.sensor == CHILD_ID_RGB_LIGHT)
  {
    // Ignore all the messages when MQ2 visual alarm is active. Logic is reverse false or 0 = GAS = DANGER!
#ifdef VISUAL_ALARM
    if (enable_visual_alarm && !mq2_d_alarm)
    {
      return;
    }
#endif
    // Extract and process RGB values
    if (message.type == V_RGB)
    {
      // Read hex string
      String rgbHexString = message.getString();
      rgbHexString.toCharArray(rgb_char, sizeof(rgb_char));
      long rgb_number = (long)strtol(&rgbHexString[0], NULL, 16);
      set_RGB_values[0] = rgb_number >> 16;
      set_RGB_values[1] = rgb_number >> 8 & 0xFF;
      set_RGB_values[2] = rgb_number >> rgb_number & 0xFF;
      // Save to EEPROM position 2,3,4 the value of R, G, B
#ifdef SAVE_STATE_TO_EEPROM
      saveState(2, set_RGB_values[0]);
      saveState(3, set_RGB_values[1]);
      saveState(4, set_RGB_values[2]);
#endif
#ifdef MY_DEBUG
      Serial.print("Received RGB LAMP RGB string message: ");
      Serial.println(message.getString());
#endif
    }
    // Extract and process RGB LAMP STATUS
    if (message.type == V_STATUS)
    {
      set_rgb_lamp_status = (message.getInt() ? true : false);
#ifdef MY_DEBUG
      Serial.print("Received RGB LAMP V_STATUS: ");
      Serial.println(message.getInt());
#endif
    }
    // Extract and process RGB LAMP DIMMER
    if (message.type == V_PERCENTAGE)
    {
      set_rgb_lamp_dimmer_percent = constrain(message.getInt(), 0, 100);
      // Save to EEPROM position 5 the value of set dimmer
#ifdef SAVE_STATE_TO_EEPROM
      saveState(5, set_rgb_lamp_dimmer_percent);
#endif
#ifdef MY_DEBUG
      Serial.print("Received RGB LAMP V_PERCENTAGE: ");
      Serial.println(message.getInt());
#endif
    }
  }
#endif
}

// ********************END OF MYSESNSORS RECEIVE FUNCTION ***************************/

// ******************** MYSESNSORS PRESENTANTION FUNCTION ***************************/

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("d-diot Nano Repeater", "1.0", ack);
// Register all sensors to gw (they will be created as child devices)
#ifdef CHILD_ID_FRONT_PIR
  present(CHILD_ID_FRONT_PIR, S_MOTION, "Front Motion Sensor", ack);
#endif
#ifdef CHILD_ID_LEFT_PIR
  present(CHILD_ID_LEFT_PIR, S_MOTION, "Left Motion Sensor", ack);
#endif
#ifdef CHILD_ID_RIGHT_PIR
  present(CHILD_ID_RIGHT_PIR, S_MOTION, "Right Motion Sensor", ack);
#endif
#ifdef CHILD_ID_HUM
  present(CHILD_ID_HUM, S_HUM, "Humidity", ack);
#endif
#ifdef CHILD_ID_TEMP
  present(CHILD_ID_TEMP, S_TEMP, "Temperature", ack);
#endif
#ifdef CHILD_ID_MQ2_D
  present(CHILD_ID_MQ2_D, S_SMOKE, "Gas Alarm", ack);
#endif
#ifdef CHILD_ID_MQ2_A
  present(CHILD_ID_MQ2_A, S_AIR_QUALITY, "Air Quality", ack);
#endif
#ifdef CHILD_ID_BUZZER
  present(CHILD_ID_BUZZER, S_SOUND, "Buzzer", ack);
#endif
#ifdef CHILD_ID_RGB_LIGHT
  present(CHILD_ID_RGB_LIGHT, S_RGB_LIGHT, "Neopixel RGB Lamp", ack);
#endif
#ifdef CHILD_ID_RGBW_LIGHT
  present(CHILD_ID_RGBW_LIGHT, S_RGBW_LIGHT, "RGBW Lamp", ack);
#endif
#ifdef CHILD_ID_LIGHT_LEVEL
  present(CHILD_ID_LIGHT_LEVEL, S_LIGHT_LEVEL, "Light Level", ack);
#endif
#ifdef CHILD_ID_ENABLE_SOUND_ALARM
  present(CHILD_ID_ENABLE_SOUND_ALARM, S_BINARY, "Enable sound alarm", ack);
#endif
#ifdef CHILD_ID_ENABLE_VISUAL_ALARM
  present(CHILD_ID_ENABLE_VISUAL_ALARM, S_BINARY, "Enable visual alarm", ack);
#endif
}

// ******************** END OF MYSESNSORS PRESENTANTION FUNCTION ************************/

// ********************************** ARDUINO SETUP *************************************/

void setup()
{
  // PIN setup
#ifdef BUZZER_PIN
  pinMode(BUZZER_PIN, OUTPUT);
#endif
#ifdef BUTTON_PIN
  pinMode(BUTTON_PIN, INPUT);
#endif
#ifdef FRONT_PIR_PIN
  pinMode(FRONT_PIR_PIN, INPUT);
#endif
#ifdef LEFT_PIR_PIN
  pinMode(LEFT_PIR_PIN, INPUT);
#endif
#ifdef RIGHT_PIR_PIN
  pinMode(RIGHT_PIR_PIN, INPUT);
#endif
#ifdef MQ2_DIGITAL_PIN
  pinMode(MQ2_DIGITAL_PIN, INPUT);
#endif
#ifdef PHOTORES_PIN
  pinMode(PHOTORES_PIN, INPUT);
#endif
#ifdef MQ2_ANALOG_PIN
  pinMode(MQ2_ANALOG_PIN, INPUT);
#endif
#ifdef RGBW_R_PIN
#ifdef RGBW_G_PIN
#ifdef RGBW_B_PIN
#ifdef RGBW_W_PIN
  pinMode(RGBW_R_PIN, OUTPUT);
  pinMode(RGBW_G_PIN, OUTPUT);
  pinMode(RGBW_B_PIN, OUTPUT);
  pinMode(RGBW_W_PIN, OUTPUT);
#endif
#endif
#endif
#endif

#ifdef DHT_DATA_PIN
  // DHT22 setup
  dht.setup(DHT_DATA_PIN);
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod())
  {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
#endif

#ifdef CHILD_ID_RGB_LIGHT
  // Neopixel initializtion
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(STRIP_BRIGHTNESS);
  // Read the RGB values in EEPROM
#ifdef SAVE_STATE_TO_EEPROM
  set_RGB_values[0] = loadState(2);
  set_RGB_values[1] = loadState(3);
  set_RGB_values[2] = loadState(4);
  uint8_t eeprom_pos5 = loadState(5);
  if (eeprom_pos5 >= 0 && eeprom_pos5 <= 100)
  {
    set_rgb_lamp_dimmer_percent = (set_rgb_lamp_status ? eeprom_pos5 : 0);
  }
#endif
#ifdef MY_DEBUG
  Serial.print("EEPROM pos 2 =");
  Serial.println(set_RGB_values[0]);
  Serial.print("EEPROM pos 3 =");
  Serial.println(set_RGB_values[1]);
  Serial.print("EEPROM pos 4 =");
  Serial.println(set_RGB_values[2]);
  Serial.print("EEPROM pos 5 =");
  Serial.println(set_rgb_lamp_dimmer_percent);
#endif
#endif

// Restore state from eeprom
#ifdef SOUND_ALARM
#ifdef SAVE_STATE_TO_EEPROM
  uint8_t eeprom_pos0 = loadState(0);
  if (eeprom_pos0 == 0 || eeprom_pos0 == 1)
  {
    enable_sound_alarm = (eeprom_pos0 ? true : false);
  }
#endif
#ifdef MY_DEBUG
  Serial.print("EEPROM pos 0 =");
  Serial.println(eeprom_pos0);
#endif
#endif

#ifdef VISUAL_ALARM
#ifdef SAVE_STATE_TO_EEPROM
  uint8_t eeprom_pos1 = loadState(1);
  if (eeprom_pos1 == 0 || eeprom_pos1 == 1)
  {
    enable_visual_alarm = (eeprom_pos1 ? true : false);
  }
#endif
#ifdef MY_DEBUG
  Serial.print("EEPROM pos 1 =");
  Serial.println(eeprom_pos1);
#endif
#endif
}

// ***************************** END OF ARDUINO SETUP ******************************

// ******************************* ARDUINO LOOP ************************************
void loop()
{
  current_time = millis();

// Front PIR
#ifdef CHILD_ID_FRONT_PIR
  if (digitalRead(FRONT_PIR_PIN) != front_pir || is_first_run)
  {
    if (is_first_run)
    {
      send(msgFrontPir.set(front_pir), ack);
    }
    else
    {
#ifdef CHILD_ID_LIGHT_LEVEL
      update_light_level();
#endif
      front_pir = !front_pir;
      send(msgFrontPir.set(front_pir), ack);
    }
#ifdef MY_DEBUG
    Serial.print("Front PIR: ");
    Serial.println(front_pir);
#endif
  }
#endif

// Left PIR
#ifdef CHILD_ID_LEFT_PIR
  if (digitalRead(LEFT_PIR_PIN) != left_pir || is_first_run)
  {
    if (is_first_run)
    {
      send(msgLeftPir.set(left_pir), ack);
    }
    else
    {
#ifdef CHILD_ID_LIGHT_LEVEL
      update_light_level();
#endif
      left_pir = !left_pir;
      send(msgLeftPir.set(left_pir), ack);
    }
#ifdef MY_DEBUG
    Serial.print("Left PIR: ");
    Serial.println(left_pir);
#endif
  }
#endif

// Right PIR
#ifdef CHILD_ID_RIGHT_PIR
  if (digitalRead(RIGHT_PIR_PIN) != right_pir || is_first_run)
  {
    if (is_first_run)
    {
      send(msgRightPir.set(right_pir), ack);
    }
    else
    {
#ifdef CHILD_ID_LIGHT_LEVEL
      update_light_level();
#endif
      right_pir = !right_pir;
      send(msgRightPir.set(right_pir), ack);
    }
#ifdef MY_DEBUG
    Serial.print("Right PIR: ");
    Serial.println(right_pir);
#endif
  }
#endif

// MQ2 Digital Alarm
#ifdef CHILD_ID_MQ2_D
  if (digitalRead(MQ2_DIGITAL_PIN) != mq2_d_alarm || is_first_run)
  {
    if (is_first_run)
    {
      // Logic is reversed
      send(msgGasD.set(mq2_d_alarm ? 0 : 1), ack);
    }
    else
    {
      mq2_d_alarm = !mq2_d_alarm;
#ifdef SOUND_ALARM
      if (enable_sound_alarm)
      {
        mq2_trigger_sound_alarm = true;
      }
#endif
#ifdef VISUAL_ALARM
      if (enable_visual_alarm)
      {
        mq2_trigger_visual_alarm = true;
      }
#endif
      // Send feedback but reverse the value because LOW = GAS DANGER!, HIGH = OK
      send(msgGasD.set(mq2_d_alarm ? 0 : 1), ack);
    }
#ifdef MY_DEBUG
    Serial.print("MQ2 Digital alarm (reversed value): ");
    Serial.println(mq2_d_alarm ? 0 : 1);
#endif
  }
#endif

// Acustic GAS alarm
#ifdef SOUND_ALARM
  // Send status message for the first run
  if (is_first_run)
  {
    send(msgSoundAlarm.set(enable_sound_alarm ? 1 : 0), ack);
#ifdef MY_DEBUG
    Serial.print("Enable sound alarm: ");
    Serial.println(enable_sound_alarm);
#endif
  }
  // Do something only if sound alarm is enable
  if (enable_sound_alarm)
  {
    // Updated the status of the Buzzer switch when the allarm status is changed
    if (mq2_trigger_sound_alarm)
    {
      buzzer_update(true);
      mq2_trigger_sound_alarm = false;
    }
    // Logic is reveresed
    if (!mq2_d_alarm)
    {
      buzzer_pulse();
    }
  }
#endif

// Visual GAS alarm
#ifdef VISUAL_ALARM
  // Send status message for the first run
  if (is_first_run)
  {
    send(msgVisualAlarm.set(enable_visual_alarm ? 1 : 0), ack);
#ifdef MY_DEBUG
    Serial.print("Enable visual alarm: ");
    Serial.println(enable_visual_alarm);
#endif
  }
  // Do something only if visual alarm is enable
  if (enable_visual_alarm)
  {
    // Updated the status of the Buzzer switch when the allarm status is changed
    if (mq2_trigger_visual_alarm)
    {
      // Logic is reveresed
      if (!mq2_d_alarm)
      {
        // Save pre-alarm LAMP values
        prealarm_R_value = set_RGB_values[0];
        prealarm_G_value = set_RGB_values[1];
        prealarm_B_value = set_RGB_values[2];
        prealarm_dimmer_percentage = set_rgb_lamp_dimmer_percent;
        prealarm_lamp_status = set_rgb_lamp_status;
        // set alarm LAMP parameters
        set_RGB_values[0] = LIGHT_RED_VALUE;
        set_RGB_values[1] = LIGHT_GREEN_VALUE;
        set_RGB_values[2] = LIGHT_BLU_VALUE;
        set_rgb_lamp_status = true;
        set_rgb_lamp_dimmer_percent = LIGHT_DIMMER_PERCENT;
      }
      else
      {
        // Restore pre-alarm lamp parameters
        set_RGB_values[0] = prealarm_R_value;
        set_RGB_values[1] = prealarm_G_value;
        set_RGB_values[2] = prealarm_B_value;
        set_rgb_lamp_status = prealarm_lamp_status;
        set_rgb_lamp_dimmer_percent = (set_rgb_lamp_status ? prealarm_dimmer_percentage : 0);
      }
      mq2_trigger_visual_alarm = false;
    }
    if (!mq2_d_alarm)
    {
      if (lamp_on_start_time == 0 || current_time > lamp_on_start_time + LIGHT_OFF_PERIOD + LIGHT_ON_PERIOD)
      {
        // Turn LAMP on
        for (uint8_t i = 0; i < NUMPIXELS; i++)
        {
          strip.setPixelColor(i, last_R_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255, last_G_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255, last_B_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255);
        }
        strip.show();
        lamp_on_start_time = current_time;
      }
      else if (current_time > LIGHT_ON_PERIOD + lamp_on_start_time)
      {
        // Turn LAMP OFF
        strip.clear();
        strip.show();
      }
    }
  }
#endif

// Buzzer
#ifdef CHILD_ID_BUZZER
  if (buzzer_set_status != buzzer_status || is_first_run)
  {
    if (is_first_run)
    {
      send(msgBuzzer.set(buzzer_status ? 1 : 0), ack);
    }
    buzzer_update();
  }
#endif

// RGB lamp
#ifdef CHILD_ID_RGB_LIGHT
  if (set_rgb_lamp_status != last_rgb_lamp_status)
  {
    last_rgb_lamp_status = set_rgb_lamp_status;
    trigger_show_strip = true;
  }
  if (last_R_value != set_RGB_values[0] || last_G_value != set_RGB_values[1] || last_B_value != set_RGB_values[2])
  {
    last_R_value = set_RGB_values[0];
    last_G_value = set_RGB_values[1];
    last_B_value = set_RGB_values[2];
    trigger_show_strip = true;
  }
  if (set_rgb_lamp_dimmer_percent != last_rgb_lamp_dimmer_percent)
  {
    last_rgb_lamp_dimmer_percent = set_rgb_lamp_dimmer_percent;
    // Turn lamp off if dimmer is 0
    if (last_rgb_lamp_dimmer_percent == 0)
    {
      last_rgb_lamp_status = false;
      set_rgb_lamp_status = false;
    }
    // turn lamp on if dimmer is not 0
    else
    {
      last_rgb_lamp_status = true;
      set_rgb_lamp_status = true;
    }
    trigger_show_strip = true;
  }
  // Force dimmer to 100 if the lamp is set to on and dimmer % is 0
  if (last_rgb_lamp_dimmer_percent == 0 && last_rgb_lamp_status)
  {
    last_rgb_lamp_dimmer_percent = 100;
    set_rgb_lamp_dimmer_percent = 100;
  }
  // Apply changes and send new values to gateway
  if (trigger_show_strip)
  {
    if (!last_rgb_lamp_status)
    {
      strip.clear();
    }
    else
    {
      for (uint8_t i = 0; i < NUMPIXELS; i++)
      {
        strip.setPixelColor(i, last_R_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255, last_G_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255, last_B_value * map(last_rgb_lamp_dimmer_percent, 0, 100, 0, 255) / 255);
      }
    }
    strip.show();
    rgb_to_hex_array();
    send(msgRGB_Status.set(last_rgb_lamp_status ? 1 : 0), ack);
    send(msgRGB_Color.set(rgb_char), ack);
    send(msgRGB_Dimmer.set(last_rgb_lamp_dimmer_percent), ack);
    trigger_show_strip = false;
#ifdef MY_DEBUG
    Serial.print("RGB Lamp staus: ");
    Serial.println(last_rgb_lamp_status);
    Serial.print("RGB Lamp RGB: ");
    Serial.println(String(rgb_char));
    Serial.print("RGB Lamp dimmer percent: ");
    Serial.println(last_rgb_lamp_dimmer_percent);
#endif
  }
  if (is_first_run)
  {
    send(msgRGB_Status.set(last_rgb_lamp_status ? 1 : 0), ack);
    rgb_to_hex_array();
    send(msgRGB_Color.set(rgb_char), ack);
    send(msgRGB_Dimmer.set(last_rgb_lamp_dimmer_percent), ack);
#ifdef MY_DEBUG
    Serial.print("RGB Lamp staus: ");
    Serial.println(last_rgb_lamp_status);
    Serial.print("RGB Lamp RGB: ");
    Serial.println(String(rgb_char));
    Serial.print("RGB Lamp dimmer percent: ");
    Serial.println(last_rgb_lamp_dimmer_percent);
#endif
  }
#endif

  // Read sensosrs values according to UPDATE_INTERVAL
  if (current_time > last_update_time + UPDATE_INTERVAL || last_update_time == 0)
  {
#ifdef CHILD_ID_LIGHT_LEVEL
    update_light_level();
#endif
#ifdef DHT_DATA_PIN
    update_temp_and_hum();
#endif
#ifdef CHILD_ID_MQ2_A
    update_gas_level(0);
#endif
    last_update_time = current_time;
  }
  is_first_run = false;
}

// **************************** END OF LOOP *********************************