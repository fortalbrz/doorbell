//------------------------------------------------------------------------------------------------------------------
//
// DOOR BELL - Doorbell with Home Assistant
// NodeMCU 0.9 (ESP-12 module)
//⠀⠀⠀            ⠀⣀⡀
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡛⢻⣧
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣟⢷⣄
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣆⠹⣆
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⡄⢹⡄
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⡇⠈⣷
//⠀       ⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣇⠀⢻⣇
//⠀      ⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠻⣧⡀
//⠀      ⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠹⣷
//⠀      ⠀⠙⠛⠿⠿⢿⣿⣿⣿⣿⣿⣿⡿⠷⠶⠚⠋
//
//------------------------------------------------------------------------------------------------------------------
// source code: https://github.com/fortalbrz/aquarioino/etc/
//
// Description:
//   Simple doorbell with NodeMCU: can use a push button as doorbell with MQTT notifications (e.g., for a home assistant).
//   - This "NodeMCU doorbell" can be connected in parallel to the push button of an alread installed doorbell.
//   - The doorbell can play the star wars theme.
//   - The doorbell has 4 relays that can be used for: open a front door, turn on external lights, etc
//   - An internal "open front door" push button is also provided (this function can be enabled or disabled MQTT commands)
//   - If the intenal "open front door" push button is pressed when it's disabled, and MQTT "alarm" notification is send
//
// Materials:
// - 1 NodeMCU 0.9 (ESP-12 module)
// - 1 relay module with 4 channels (or 2 channels) - optional
// - 1 power source 5v (1 A)
// - 1 tactile push buttom keys and 1 x resistor 10k olhms (resistor "A")
// - 1 buzzer (5V), 1 resistor 1k olhms (resistor "B") and 1 transistor BC548 [1]
// - 1 led and resistor 10k olhms (resistor "C") (optional, indicates "power on")
// - 1 electrolytic capacitor 100 uF (optional)
// - flexible cab (22 agw)
//
//                     (door bell)
//   -------------    [push button]---Gnc   
//  |             |        |
//  |   NodeMCU   D1 ------x-------[10k olhms]-- Vcc
//  |    ESP-12   D2 --------------------------- Relay "Door Open" (ch1)
//  |             D4 --------------------------- Relay "04" (ch4) - optional
//  |             D5 --------------------------- Relay "External Light" (ch2)
//  |             D6 --------------------------- Relay "03" (ch3) - optional
//  |             D7 ---[1k olhm]--x
//  |             |                | BC548
//   -------------               -----
//                             E /   \ B 
//                            (gnd)   x------ [Buzzer]---Vcc
//
// Circuit Wiring Instruction:
//  - NodeMCU "Vin" --> +5 V power source (VCC)
//  - NodeMCU "Gnd" --> -5 V power source (GND)
//  - NodeMCU "D1" --> resistor 10k olhms "A" terminal 1
//  - resistor 10k olhms "A" terminal 2 --> +5 V power source (VCC) 
//  - Arduino Nano "D1" --> tactile push buttom "DOORBELL" terminal 1 (NC)
//  - tactile push buttom "DOORBELL" terminal 2 (NC) --> -5 V power source (GND)
//  - Relay 2 ch "VCC" --> +5 V power source (VCC)
//  - Relay 2 ch "GND" --> -5 V power source (GND)
//  - NodeMCU "D2" --> Relay 4 ch "IN 1" (Door Locker)
//  - NodeMCU "D5" --> Relay 4 ch "IN 2" (Front Light)
//  - NodeMCU "D6" --> Relay 4 ch "IN 3" (general purpose)
//  - NodeMCU "D4" --> Relay 4 ch "IN 4" (general purpose)
//  - NodeMCU "D7" --> resistor 1k olhms "B" terminal 1
//  - resistor 1k olhms "B" terminal 2 --> transistor BC548 base (pin 2 - middle)
//  - transistor BC548 emitter (pin 3 - right) --> -5 V power source (GND)
//  - transistor BC548 base (pin 1 - left) --> buzzer terminal 1
//  - buzzer terminal 2 --> +5 V power source (VCC)
//  - Led terminal 1 (positive) --> +5 V power source (VCC) (optional, "power on led")
//  - Led terminal 2 (negative/bevel) --> resistor 10k olhms "C" terminal 1 (optional, "power on led")
//  - resistor 10k olhms "C" terminal 2 --> -5 V power source (GND) (optional, "power on led")
//  - capacitor 100uF (positive) --> +5 V power source (VCC) (optional)
//  - capacitor 100uF (negative/"minus sign") --> resistor 10k olhms "C" terminal 2 (optional)
//
// Flashing the code:
//  - the NodeMUC may use the USB/serial IC CH340G, so it's necessary to install the Windows driver: 
//      - CH340G driver: http://bit.ly/44WdzVF (windows 11 compatible)
//      - driver installation instructions (pt-BR): http://bit.ly/3ZqIqc0
//  - download Arduino IDE (ver 2.0): https://www.arduino.cc/en/software
//  - install WifiEsp library:
//      - On arduino IDE select: "File" -> "Preferences" -> "Additional boards manager URL":
//         - add the URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
//      - On arduino IDE select: "Tools" -> "Manage Libraries":
//         - search and install: "WifiEsp" (by bportaluri)
//         - search and install: "PubSubClient" (by Nick)
//  - uses a micro USB cable to connect to the Arduino
//  - select the "NodeMCU 0.9 (ESP-12 module)" board in the arduino IDE: "Tools" -> "Board".
//  - select "Sketch" -> "Upload"
//
// Serial Monitor:
//  sets the macro "DEBUG_MODE" as true in order to debug on serial monitor (testing only)
//
// Setup:
//  WIFI_SSID                              Wi-fi SSID
//  WIFI_PASSWORD                          Wi-fi password
//  MQTT_BROKER_ADDRESS                    MQTT broker server ip
//  MQTT_BROKER_PORT                       MQTT broker port
//  MQTT_USERNAME                          MQTT username, can be omitted if not needed
//  MQTT_PASSWORD                          MQTT password, can be omitted if not needed
//  DEBUG_MODE                             enables/disables debug on serial monitor (testing only)
//  USE_BUZZER                             enables/disables buzzer code (disable to not use a buzzer)
//  PLAY_TUNES                             enables/disables music tunes on doorbell press (requires a buzzer)
//  DOORBELL_TUNE                          doorbell tune: -1: random, 0: star wars, 1: darth vader, 2: tetris, 
//                                         3: super mario bros, 4: godfather (requires a buzzer)
//
// MQTT topics and commands:
//  - state topics:
//     - "doorbell/ring": "on" for doorbell ringing
//     - "doorbell/alarm": "on" for alarm status: the physical "open front door" button was pressed when it was disabled
//     - "doorbell/status": status topic (JSON):
//           {"light": "on/off",     // light state (relay #02) 
//            "relay3":, "on/off",  // relay #03 state
//            "relay4", "on/off",    // relay #03 state
//            "enabled", "on/off",  // enables/disables physical open door button
//            "running\" "on/off"}   // running state
//     - "doorbell/available": "online/offline" - MQTT topic for availability notification (home assistant "unavailable" state)
//  - command topic:
//     - "doorbell/cmd":
//           "open door": opens the front door (relay #01)
//           "light on": turn on the external lights (relay #02)
//           "light off": turn off the external lights (relay #02)
//           "relay3 on": turn on the relay #03
//           "relay3 off": turn off the relay #03
//           "relay4 on": turn on the relay #04
//           "relay4 off": turn off the relay #04
//           "open door on": enable physical fort door open button
//           "open door off": disable physical fort door open button
//           "play": testing command: plays a random tune
//           "ring": testing command: emulates doorbell press 
//
// Remarks:
//  NodeMCU pins D3 and D4 must be pull-up: boot fails on low, thus this are required high on boot.
//  NodeMCU pin D4 is connected to build-in LED: do not use BUILTIN led.
//
// References:
// [1] https://medium.com/@arthurmehl/como-usar-um-buzzer-no-nodemcu-40b46973e99b
// [2] https://www.makerhero.com/blog/controle-monitoramento-iot-nodemcu-e-mqtt/
// [3] https://projecthub.arduino.cc/tmekinyan/playing-popular-songs-with-arduino-and-a-buzzer-546f4a
// [4] https://github.com/robsoncouto/arduino-songs/blob/master/starwars/starwars.ino
// [5] https://newbiely.com/tutorials/esp8266/esp8266-pinout
//
//------------------------------------------------------------------------------------------------------------------
//
// Jorge Albuquerque (2024) - https://linkedin.com/in/jorgealbuquerque
// https://www.jorgealbuquerque.com
//
//------------------------------------------------------------------------------------------------------------------
#define DEBUG_MODE false                                    // enables/disables serial monitor debugging messages
#define WIRING_TEST_MODE false                              // enables/disables wiring testing (debug only)
#define USE_BUZZER true                                     // enables/disables buzzer code (disable to not use a buzzer)
#define PLAY_TUNES true                                     // enables/disables music tunes on door bell press (requires a buzzer)
#define DOORBELL_TUNE -1                                    // doorbell tune: -1: random, 0: star wars, 1: darth vader, 2: tetris, 3: super mario bros, 4: godfather (requires a buzzer)
// Wi-fi setup
#define WIFI_SSID "jorge cps"                               // Wi-fi SSID
#define WIFI_PASSWORD "casa1976bonita1980"                  // Wi-fi password
// MQTT setup
#define MQTT_BROKER_ADDRESS "192.168.68.10"                 // MQTT broker server ip
#define MQTT_BROKER_PORT 1883                               // MQTT broker port
#define MQTT_USERNAME "mqtt-user"                           // can be omitted if not needed
#define MQTT_PASSWORD "mqtt"                                // can be omitted if not needed
// MQTT topics
#define MQTT_RINGING_TOPIC "doorbell/ring"                 // MQTT topic for doorbell ringing notification
#define MQTT_COMMAND_TOPIC "doorbell/cmd"                  // MQTT topic for send door commands (e.g., open from door)
#define MQTT_STATUS_TOPIC "doorbell/status"                // MQTT topic for doorbell status
#define MQTT_AVAILABILITY_TOPIC "doorbell/available"       // MQTT topic for availability notification (home assistant "unavailable" state)
#define MQTT_ALARM_TOPIC "doorbell/alarm"                  // MQTT topic for alarm status
#define MQTT_DEVICE_ID  "doorbell_1fmo43iowerwe23"         // MQTT session identifier
#define USE_ALARM_TOPIC false                              // enables/disables the topic for alarm status
//
// MQTT commands
//
#define MQTT_COMMAND_OPEN_DOOR "open door"                  // MQTT command to open the front door
#define MQTT_COMMAND_TURN_LIGHT_ON "light on"               // MQTT command to turn on the external lights
#define MQTT_COMMAND_TURN_LIGHT_OFF "light off"             // MQTT command to turn off the external lights
#define MQTT_COMMAND_RELAY_03_ON "relay3 on"                // MQTT command to turn on the relay #3
#define MQTT_COMMAND_RELAY_03_OFF "relay3 off"              // MQTT command to turn off the relay #3
#define MQTT_COMMAND_RELAY_04_ON "relay4 on"                // MQTT command to turn on the relay #4
#define MQTT_COMMAND_RELAY_04_OFF "relay4 off"              // MQTT command to turn off the relay #4
#define MQTT_COMMAND_ENABLE_DOOR_OPEN_ON "open door on"     // MQTT command to enable physical fort door open button
#define MQTT_COMMAND_ENABLE_DOOR_OPEN_OFF "open door off"   // MQTT command to disable physical fort door open button
#define MQTT_COMMAND_DEBUG_PLAY "play"                      // testing command: play random music
#define MQTT_COMMAND_DEBUG_PLAY_STAR_WARS "play sw"         // testing command: play star wars theme
#define MQTT_COMMAND_DEBUG_PLAY_DARTH_VADER "play dv"       // testing command: play darth vader theme
#define MQTT_COMMAND_DEBUG_PLAY_TETRIS "play tetris"        // testing command: play tetris theme
#define MQTT_COMMAND_DEBUG_PLAY_MARIO "play mario"          // testing command: play mario theme
#define MQTT_COMMAND_DEBUG_PLAY_GOT "play got"              // testing command: play game of thrones theme
#define MQTT_COMMAND_DEBUG_PLAY_GODFATHER "play gf"         // testing command: play godfather theme
#define MQTT_COMMAND_DEBUG_RING "ring"                      // testing command: emulates doorbell press 
#define MQTT_COMMAND_DEBUG_REFRESH "refresh"                // testing command: update MQTT status
// timers
#define MQTT_STATUS_RINGING_TIME 30000                      // MQTT ring state duration, in miliseconds (default: 30 seconds)
#define MQTT_STATUS_UPDATE_TIME 300000                      // time for send and status update (default: 5 min)
#define MQTT_AVAILABILITY_TIME  60000                       // elapsed time to send MQTT availability, in miliseconds (default: 1 min)
#define BUTTON_SINGLE_PUSH_TIME 3000                        // time to avoid double front door push button press
#define DOOR_RELAY_POWER_ON_TIME 1500                       // Font door relay power on time (pulsed)
#define BUZZER_POWER_ON_TIME 800                            // buzzen power on time when the doorbell button is pressed (PLAY_TUNES as false)
#define SERIAL_BAUDRATE 9600                                // serial monitor baud rate (only for debuging)
//
// NodeMCU pinout
//
#define DOORBELL_BUTTON_PIN D1
#define DOOR_RELAY_PIN D2
#define OPEN_DOOR_BUTTON_PIN D3                       // (remark: D3 must be pull-up - boot fails on low, thus required high)
#define RELAY_04_PIN D4                               // (remark: D4 must be pull-up - boot fails on low, thus required high - connected to build-in LED)
#define LIGHT_RELAY_PIN D5
#define RELAY_03_PIN D6
#define BUZZER_PIN D7 

//------------------------------------------------------------------------------------------------------------------

// librarys (see doc above)
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
 
WiFiClient espClient;
PubSubClient MQTT(espClient);

// globals
bool _ringing = false; // ringing state
bool _doorOn = false; // front door opening state
bool _lightOn = false; // external light state
bool _relay3On = false; // relay 3 state
bool _relay4On = false; // relay 4 state
bool _enablesDoorOpen = true; // enables/disables the physical button to open the front door
unsigned long _lastAvailabilityTime = 0;
unsigned long _lastStatusUpdateTime = 0;
unsigned long _lastButtonPressTime = 0;
unsigned long _lastRingTime = 0;

#if (USE_BUZZER == true && PLAY_TUNES == true)
  /* ----------------------------------------------------------------------
    Songs available at https://github.com/robsoncouto/arduino-songs                                                                                          
                                                Robson Couto, 2019
    ----------------------------------------------------------------------*/
  #define NOTE_B0 31
  #define NOTE_C1 33
  #define NOTE_CS1 35
  #define NOTE_D1 37
  #define NOTE_DS1 39
  #define NOTE_E1 41
  #define NOTE_F1 44
  #define NOTE_FS1 46
  #define NOTE_G1 49
  #define NOTE_GS1 52
  #define NOTE_A1 55
  #define NOTE_AS1 58
  #define NOTE_B1 62
  #define NOTE_C2 65
  #define NOTE_CS2 69
  #define NOTE_D2 73
  #define NOTE_DS2 78
  #define NOTE_E2 82
  #define NOTE_F2 87
  #define NOTE_FS2 93
  #define NOTE_G2 98
  #define NOTE_GS2 104
  #define NOTE_A2 110
  #define NOTE_AS2 117
  #define NOTE_B2 123
  #define NOTE_C3 131
  #define NOTE_CS3 139
  #define NOTE_D3 147
  #define NOTE_DS3 156
  #define NOTE_E3 165
  #define NOTE_F3 175
  #define NOTE_FS3 185
  #define NOTE_G3 196
  #define NOTE_GS3 208
  #define NOTE_A3 220
  #define NOTE_AS3 233
  #define NOTE_B3 247
  #define NOTE_C4 262
  #define NOTE_CS4 277
  #define NOTE_D4 294
  #define NOTE_DS4 311
  #define NOTE_E4 330
  #define NOTE_F4 349
  #define NOTE_FS4 370
  #define NOTE_G4 392
  #define NOTE_GS4 415
  #define NOTE_A4 440
  #define NOTE_AS4 466
  #define NOTE_B4 494
  #define NOTE_C5 523
  #define NOTE_CS5 554
  #define NOTE_D5 587
  #define NOTE_DS5 622
  #define NOTE_E5 659
  #define NOTE_F5 698
  #define NOTE_FS5 740
  #define NOTE_G5 784
  #define NOTE_GS5 831
  #define NOTE_A5 880
  #define NOTE_AS5 932
  #define NOTE_B5 988
  #define NOTE_C6 1047
  #define NOTE_CS6 1109
  #define NOTE_D6 1175
  #define NOTE_DS6 1245
  #define NOTE_E6 1319
  #define NOTE_F6 1397
  #define NOTE_FS6 1480
  #define NOTE_G6 1568
  #define NOTE_GS6 1661
  #define NOTE_A6 1760
  #define NOTE_AS6 1865
  #define NOTE_B6 1976
  #define NOTE_C7 2093
  #define NOTE_CS7 2217
  #define NOTE_D7 2349
  #define NOTE_DS7 2489
  #define NOTE_E7 2637
  #define NOTE_F7 2794
  #define NOTE_FS7 2960
  #define NOTE_G7 3136
  #define NOTE_GS7 3322
  #define NOTE_A7 3520
  #define NOTE_AS7 3729
  #define NOTE_B7 3951
  #define NOTE_C8 4186
  #define NOTE_CS8 4435
  #define NOTE_D8 4699
  #define NOTE_DS8 4978
  #define REST 0

  // change this to whichever pin you want to use
  int buzzer = BUZZER_PIN;

  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  
  // star wars theme
  int melody_star_wars[] = {
    NOTE_F5, 2, NOTE_C6, 2,
    NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F6, 2, NOTE_C6, 4,
    NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F6, 2, NOTE_C6, 4,
    NOTE_AS5, 8, NOTE_A5, 8, NOTE_AS5, 8, NOTE_G5, 2
  };

  // darth vader imperial march
  int melody_darth_vader[] = {  
    NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16,
    NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16,
    NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,    
    NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
    NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,
    NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
    NOTE_C5,4, NOTE_A4,-8, NOTE_C5,16, NOTE_E5,2
  };

  // tetris
  int melody_tetris[] = {
    NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
    NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
    NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
    NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,
    NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
    NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
    NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
    NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4
  };

  // super mario bros
  int melody_mario[] = {
    NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
    NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
    NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
    NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
    NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
    REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
    NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
    NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
    NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
    REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
    
    REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
    REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
    REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
    NOTE_C5,2, REST,2,

    REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
    REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
    REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
    NOTE_C5,2, REST,2
  };

  // game of thrones theme
  int melody_game_of_thrones[] = {
    NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, //1
    NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
    NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
    NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
    NOTE_G4,-4, NOTE_C4,-4,//5

    NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
    NOTE_D4,-1, //7 and 8
    NOTE_F4,-4, NOTE_AS3,-4,
    NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
    NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1, //11 and 12
  };

  // godfather theme
  int melody_godfather[] = {
    REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //1
    NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
    NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
    NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
    
    NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
    NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
    NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8, 
    NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,
    NOTE_A4, 2, 
  };
  
  // available melodies
  int *pmelodies[]=
  {
    melody_star_wars,
    melody_darth_vader,
    melody_tetris,
    melody_mario,
    melody_game_of_thrones,
    melody_godfather
  };

  // change this to make the song slower or faster
  int tempo[] = 
  {
    108, 
    120, 
    144, 
    200,
    85,
    80
  };

  int notes[] = 
  {
    sizeof(melody_star_wars) / sizeof(melody_star_wars[0]) / 2,
    sizeof(melody_darth_vader) / sizeof(melody_darth_vader[0]) / 2,
    sizeof(melody_tetris) / sizeof(melody_tetris[0]) / 2,
    sizeof(melody_mario) / sizeof(melody_mario[0]) / 2,
    sizeof(melody_game_of_thrones) / sizeof(melody_game_of_thrones[0]) / 2,
    sizeof(melody_godfather) / sizeof(melody_godfather[0]) / 2    
  };

#endif


//------------------------------------------------------------------------------------------------------------------
//
// prototypes
//
//------------------------------------------------------------------------------------------------------------------
void connectWiFi();
void connectMQTT();
void onMessage(char* topic, byte* payload, unsigned int length);
void updateStates();
ICACHE_RAM_ATTR void onDoorBell();
ICACHE_RAM_ATTR void onOpenDoor();
void openDoor();
void setRelay(const unsigned int& relayPin, const bool& state);
void ring(const bool& state);
void playTune(unsigned int song);
void beep(const uint8_t& n, const unsigned long& time);
void wiringTest();
String toStr(const char* label, const bool& state, const bool& end);

//------------------------------------------------------------------------------------------------------------------
//
// Functions
//
//------------------------------------------------------------------------------------------------------------------

void setup() {
  //
  // initialization
  // 
  #if (DEBUG_MODE == true)
    // serial monitor
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(F(" "));
    Serial.println(F("-- Debug Mode --"));
  #endif
  
  // push buttons pins
  pinMode(DOORBELL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(OPEN_DOOR_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DOORBELL_BUTTON_PIN), onDoorBell, FALLING);
  attachInterrupt(digitalPinToInterrupt(OPEN_DOOR_BUTTON_PIN), onOpenDoor, FALLING);
  
  // door locker relay pin (pulsed)
  pinMode(DOOR_RELAY_PIN, OUTPUT);
  digitalWrite(DOOR_RELAY_PIN, HIGH);

  // front light relay pin
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  digitalWrite(LIGHT_RELAY_PIN, HIGH);

  // relay #3: general purpose
  pinMode(RELAY_03_PIN, OUTPUT);
  digitalWrite(RELAY_03_PIN, HIGH);

  // relay #4: general purpose
  pinMode(RELAY_04_PIN, OUTPUT);
  digitalWrite(RELAY_04_PIN, HIGH);

  // buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  // starting beep
  beep(1, 100);
  delay(100);

  // wifi connect    
  #if (DEBUG_MODE == true)
    Serial.print(F("Connecting to wifi network: "));
    Serial.println(WIFI_SSID);     
  #endif
  
  // Wi-fi connection
  connectWiFi();

  // MQTT broker setup
  MQTT.setServer(MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);
  #if (WIRING_TEST_MODE == false)
    MQTT.setCallback(onMessage);
  #endif
}

void loop() {
  //
  // main loop
  //
  #if (WIRING_TEST_MODE == true)
    // wiring test routine
    wiringTest();
    delay(2000);
    return;
  #endif
  //
  // keeps MQTT connection alive    
  //
  if (!MQTT.connected()) 
      connectMQTT();

  if ((millis() - _lastAvailabilityTime) > MQTT_AVAILABILITY_TIME) {
    // sends MQTT "availability" message
    MQTT.publish(MQTT_AVAILABILITY_TOPIC, "online");
    _lastAvailabilityTime = millis();
  }

  if (_ringing && (millis() - _lastRingTime) > MQTT_STATUS_RINGING_TIME) {
    // clears the MQTT "ring" state after a minute
    ring(false);
  }
      
  if ((millis() - _lastStatusUpdateTime) > MQTT_STATUS_UPDATE_TIME) {
    // status update (with max timespan for 5 minutes)
    updateStates();
    _lastStatusUpdateTime = millis();
  }

  // MQTT loop
  MQTT.loop();

  // waits
  delay(1000);

  // keeps wifi connected
  connectWiFi();
}

//--------------------------------------------------------------------------------------------------
//
// ISR (push button event handlers)
//
//--------------------------------------------------------------------------------------------------


void onDoorBell() {
  //
  // On doorbell button pressed: publish MQTT status (ringing!)
  // 
  if ((millis() - _lastButtonPressTime) > BUTTON_SINGLE_PUSH_TIME) {
    // avoid double button press!
    if (MQTT.connected()) {
      // publishes the "ring" status on MQTT topic                            
      ring(true);        
      // plays some random tune!
      #if (DOORBELL_TUNE >= 0)
        playTune(DOORBELL_TUNE);
      #else
        int song = random(0, 5);
        playTune(song);
      #endif      
      _lastButtonPressTime = millis();    
    }
  }
}

void onOpenDoor() {
  //
  // On "open door" button pressed
  //
  if (_enablesDoorOpen){
    // opens the front door (if enabled)
    openDoor();  

  } else {
    #if (USE_ALARM_TOPIC == true)
      // invalid physical button press!!
      if (MQTT.connected()) {
        MQTT.publish(MQTT_ALARM_TOPIC, "on");
      }
    #endif
  }
}


//--------------------------------------------------------------------------------------------------
//
// Actions
//
//--------------------------------------------------------------------------------------------------

void ring(const bool& state = true){
  //
  // sends a MQTT doorbell "ding"
  //
  // parameters:
  //   state       send a ring "on" or "off"
  //
  if (MQTT.connected()) {        
    MQTT.publish(MQTT_RINGING_TOPIC, state ? "on" : "off");
    _ringing = state;
    if (state)
      _lastRingTime  = millis();        
  } else {
    // MQTT not conneted: cann't assume a "ringing" status!
    _ringing = false;
  }
}

void openDoor() {
  //
  // Opens the front door (pulsed)
  //
  if (_enablesDoorOpen) {
    setRelay(DOOR_RELAY_PIN, true);
    delay(DOOR_RELAY_POWER_ON_TIME);
    setRelay(DOOR_RELAY_PIN, false);
  }
}

//--------------------------------------------------------------------------------------------------
//
//  MQTT
//
//--------------------------------------------------------------------------------------------------

void onMessage(char* topic, byte* payload, unsigned int length) {
  //
  // On MQTT message
  //
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0'; // NULL;

  #if (DEBUG_MODE == true)
    Serial.print(F(" - mqtt command: ["));
    Serial.print(msg);
    Serial.println(F("]"));
  #endif
  
  // decode Home Assistant command (MQTT)
  if (strcmp(msg, MQTT_COMMAND_OPEN_DOOR) == 0){
    // opens the front door (relay #1) - 12 Vdc
    openDoor();

  } else if (strcmp(msg, MQTT_COMMAND_TURN_LIGHT_ON) == 0){
    // turns on the front door light (relay #2)
    setRelay(LIGHT_RELAY_PIN, true);

  } else if (strcmp(msg, MQTT_COMMAND_TURN_LIGHT_OFF) == 0){
    // turns off front door light (relay #2)
    setRelay(LIGHT_RELAY_PIN, false);

  } else if (strcmp(msg, MQTT_COMMAND_RELAY_03_ON) == 0){
    // turns on relay #3
    setRelay(RELAY_03_PIN, true);

  } else if (strcmp(msg, MQTT_COMMAND_RELAY_03_OFF) == 0){
    // turns off relay #3
    setRelay(RELAY_03_PIN, false);

  } else if (strcmp(msg, MQTT_COMMAND_RELAY_04_ON) == 0){
    // turns on relay #4
    setRelay(RELAY_04_PIN, true);

  } else if (strcmp(msg, MQTT_COMMAND_RELAY_04_OFF) == 0){
    // turns off relay #4
    setRelay(RELAY_04_PIN, false);

  } else if (strcmp(msg, MQTT_COMMAND_ENABLE_DOOR_OPEN_ON) == 0){
    // enables open front door
    _enablesDoorOpen = true;
    updateStates();

  } else if (strcmp(msg, MQTT_COMMAND_ENABLE_DOOR_OPEN_OFF) == 0){
    // disables open front door
    _enablesDoorOpen = false;
    updateStates();
  
  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY) == 0) {
    // plays random tune
    int song = random(0, 5);
    playTune(song);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_STAR_WARS) == 0) {
    // playd the star wars theme
    playTune(0);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_DARTH_VADER) == 0) {
    // playd the darth vader theme
    playTune(1);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_TETRIS) == 0) {
    // plays the tetris theme
    playTune(2);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_MARIO) == 0) {
    // plays the super mario bros theme
    playTune(3);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_GOT) == 0) {
    // plays the super mario bros theme
    playTune(4);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_PLAY_GODFATHER) == 0) {
    // plays the super mario bros theme
    playTune(5);    

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_RING) == 0){
    // simulates user pressing doorbell button
    ring(true);

  } else if (strcmp(msg, MQTT_COMMAND_DEBUG_REFRESH) == 0){
    // update MQTT states
    updateStates();
  }

  
}

void updateStates() {
  //
  // MQTT publish states update
  //
  if (MQTT.connected()) {
    String json = String(F("{"));
    json += toStr("light", _lightOn, false);
    json += toStr("relay3", _relay3On, false);
    json += toStr("relay4", _relay4On, false);  
    json += toStr("enabled", _enablesDoorOpen, true);

    unsigned int n = json.length() + 1;
    char message[n];
    json.toCharArray(message, n);

    #if (DEBUG_MODE == true)
      Serial.print(F("mqtt update: "));
      Serial.println(message);
    #endif

    MQTT.publish(MQTT_STATUS_TOPIC, message);
  }
}

void connectMQTT() {
  //
  // Connects to MQTT broker
  //
  while (!MQTT.connected()) {    
    #if (DEBUG_MODE == true)
      Serial.print(F("connecting to MQTT broker: "));
      Serial.println(MQTT_BROKER_ADDRESS);
    #endif        

    // retry until connection
    connectWiFi();

    if (MQTT.connect(MQTT_DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        #if (DEBUG_MODE == true)
          Serial.println(F("MQTT broker connected!"));
        #endif
        // subscribe to command topic on connection
        MQTT.subscribe(MQTT_COMMAND_TOPIC); 
        delay(50);
        MQTT.publish(MQTT_AVAILABILITY_TOPIC, "online");
        delay(50);
        updateStates();
        delay(50);
    } else {
        #if (DEBUG_MODE == true)
          Serial.println(F("Fail connecting to MQTT broker (retry in 2 secs)."));
        #endif
        beep(3, 100);
        delay(2000);
    }
  }
}

void connectWiFi() {
  //
  // connects to WiFi
  //
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // retry until connection
  while (WiFi.status() != WL_CONNECTED) 
  {    
    beep(2, 100);
    delay(1000);
    #if (DEBUG_MODE == true)
      Serial.print(F("."));
    #endif
  }

  #if (DEBUG_MODE == true)
    byte mac[6];
    WiFi.macAddress(mac);

    Serial.println();
    Serial.print(F("WiFi connected: "));
    Serial.println(WIFI_SSID);

    Serial.print(F(" - IP address: "));
    Serial.println(WiFi.localIP());

    Serial.print(F(" - MAC: "));
    Serial.print(mac[5], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.println(mac[0], HEX);
    
    Serial.print(F(" - RSSI [db]: "));
    Serial.println(WiFi.RSSI());    
  #endif
}

//--------------------------------------------------------------------------------------------------
//
// Sound feedbacks
//
//--------------------------------------------------------------------------------------------------

void playTune(unsigned int song = 0) {
  //
  // Plays the Dart Vader theme (Imperial March) on buzzer
  //
  #if (USE_BUZZER == true)
    //
    // plays some music!
    //
    #if ( PLAY_TUNES == true)
      // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
      // there are two values per note (pitch and duration), so for each note there are four bytes
      // this calculates the duration of a whole note in ms
      const int wholenote = (60000 * 4) / tempo[song];
      int divider = 0, noteDuration = 0;
        
      // iterate over the notes of the melody.
      // Remember, the array is twice the number of notes (notes + durations)
      for (int currentNote = 0; currentNote < notes[song] * 2; currentNote = currentNote + 2) {
        
        divider = pmelodies[song][currentNote + 1];

        if (divider > 0) {
          // regular note, just proceed
          noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
          // dotted notes are represented with negative durations!!
          noteDuration = (wholenote) / abs(divider);
          noteDuration *= 1.5;  // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        digitalWrite(BUZZER_PIN, HIGH);
        tone(buzzer, pmelodies[song][currentNote], noteDuration * 0.9);
              
        // Wait for the specief duration before playing the next note.
        delay(noteDuration);

        // stop the waveform generation before the next note.
        noTone(buzzer);
        digitalWrite(BUZZER_PIN, LOW);
      }
    #else
      // simple beep
      beep(200, 4);
      delay(1000);
    #endif

  #endif
}


void beep(const uint8_t& n = 1, const unsigned long& time = BUZZER_POWER_ON_TIME) {  
  //
  // plays a beep sound
  //
  // parameters:
  //     n       number of beeps
  //     time    beep duration (in miliseconds)
  //
  #if (USE_BUZZER == true)
    for (uint8_t i = 0; i < n; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(time);
      digitalWrite(BUZZER_PIN, LOW);
      if (n > 1) {
        delay(time);
      }
    }
  #endif
}

//--------------------------------------------------------------------------------------------------

void setRelay(const unsigned int& relayPin, const bool& state) {
  //
  // sets the relay states (on/off)
  //
  // parameters:
  //     relayPin       relay pin number
  //     state          true for turn on, false for turn off
  //
  delay(50);

  switch (relayPin) {

    case DOOR_RELAY_PIN: 
      // open frony door relay
      digitalWrite(relayPin, (state ? LOW : HIGH));
      _doorOn = state;
      return;      

    case LIGHT_RELAY_PIN: 
      // lightening relay
      if (_lightOn == state)
        return;
      _lightOn = state;
      #if (DEBUG_MODE == true)
        Serial.print(F(" - relay light: "));
        Serial.println(state ? F("on") : F("off"));
      #endif
      break;

    case RELAY_03_PIN: 
      if (_relay3On == state)
        return;
      _relay3On = state;
      #if (DEBUG_MODE == true)
        Serial.print(F(" - relay 3: "));
        Serial.println(state ? F("on") : F("off"));
      #endif
      break;

    case RELAY_04_PIN: 
      if (_relay4On == state)
        return;
      _relay4On = state;
      #if (DEBUG_MODE == true)
        Serial.print(F(" - relay 4: "));
        Serial.println(state ? F("on") : F("off"));
      #endif
      break;      
  }

  // relays are active LOW  
  digitalWrite(relayPin, (state ? LOW : HIGH));

  // update home assistant states
  updateStates();
}

//--------------------------------------------------------------------------------------------------

String toStr(const char* label, const bool& state, const bool& end) {
  //
  // writes a string line with the format: 
  //     "label": "on/off",
  // 
  String text = end ? String(F("\"[LABEL]\": \"[VALUE]\" }")) : String(F("\"[LABEL]\": \"[VALUE]\", "));
  text.replace(F("[LABEL]"), label);
  text.replace(F("[VALUE]"), (state? F("on") : F("off")));
  return text;
}

//--------------------------------------------------------------------------------------------------
//
// DEBUG ONLY
//
//--------------------------------------------------------------------------------------------------

void wiringTest() {
  //
  // Routine for testing wiring
  // Remark: sets WIRING_TEST_MODE as true
  //
  #if (WIRING_TEST_MODE == true)
      
    #if (DEBUG_MODE == true)
      Serial.println(F(""));
      Serial.println(F("Wiring testing"));
    #endif

    #if (DEBUG_MODE == true)
      Serial.println(F(" - testing relay ch1 (door)"));
    #endif
    openDoor();
    delay(1000);

    #if (DEBUG_MODE == true)
      Serial.println(F(" - testing relay ch2 (light)"));
    #endif
    setRelay(LIGHT_RELAY_PIN, true);
    delay(1000);
    setRelay(LIGHT_RELAY_PIN, false);
    delay(1000);

    #if (DEBUG_MODE == true)
      Serial.println(F(" - testing relay ch3 "));
    #endif
    setRelay(RELAY_03_PIN, true);
    delay(1000);
    setRelay(RELAY_03_PIN, false);
    delay(1000);

    #if (DEBUG_MODE == true)
      Serial.println(F(" - testing relay ch4 "));
    #endif
    setRelay(RELAY_04_PIN, true);
    delay(1000);
    setRelay(RELAY_04_PIN, false);
    delay(1000);

    #if (DEBUG_MODE == true)
      Serial.println(F(" - testing buzzer"));
    #endif
    playTune();


  #endif
}

//--------------------------------------------------------------------------------------------------

