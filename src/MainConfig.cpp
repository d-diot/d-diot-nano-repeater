/* d-diot nano repeater - Devices Configuration */

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

// Button configuration
#ifdef BUTTON_PIN
#define SHORT_KEY_PRESS_TIME 250
#define LONG_KEY_PRESS_TIME 1000
#define DOUBLE_CLICK_INTERVAL 250
#ifdef BUZZER_PIN
#define KEY_PRESS_FREQ 1000
#endif
#endif