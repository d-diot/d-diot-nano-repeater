/* d-diot nano repeater - MySensors Configuration */

#define MY_REPEATER_FEATURE
#define MY_SIGNING_SOFT
#define MY_SIGNING_REQUEST_SIGNATURES
#ifdef MY_SIGNING_ATSHA204_PIN
#define MY_SIGNING_ATSHA204
#endif
#define MY_RADIO_RF24
#ifdef MY_RADIO_RF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define MY_RF24_IRQ_PIN 2
#define MY_RF24_CE_PIN 7
#define MY_RF24_CS_PIN 8
#endif
#define MY_DEFAULT_ERR_LED_PIN 14
#define MY_DEFAULT_TX_LED_PIN 15
#define MY_DEFAULT_RX_LED_PIN 16
// Timeout before starting loop without gateway connection
#define MY_TRANSPORT_WAIT_READY_MS 10000
#define MY_SPLASH_SCREEN_DISABLED
static const bool ack = false;