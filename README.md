# DOORBELL - Doorbell with Home Assistant
## :house_with_garden: Doorbell Automation with Home Assistant and NodeMCU

**Optimized for NodeMCU 1.0 (ESP-12E Module)** 

                ⠀⣀⡀
               ⣾⡛⢻⣧
              ⣠⣾⣿⣟⢷⣄
             ⣸⣿⣿⣿⣿⣆⠹⣆
            ⢰⣿⣿⣿⣿⣿⣿⡄⢹⡄
            ⣿⣿⣿⣿⣿⣿⣿⡇⠈⣷
           ⣼⣿⣿⣿⣿⣿⣿⣿⣇⠀⢻⣇
        ⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠻⣧⡀
       ⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠹⣷
       ⠀⠙⠛⠿⠿⢿⣿⣿⣿⣿⣿⣿⡿⠷⠶⠚⠋




This project is aimed at those who already have a video intercom and want to integrate it with Home Assistant. In this case, the path chosen was to simply connect the NodeMCU in parallel with the push button of the video intercom that I already had, as show bellow.

![intercom](https://github.com/fortalbrz/doorbell/blob/main/img/schema.png?raw=true)

- **Video**: **[YouTube](https://youtu.be/uQc_9umpfpI)** [pt]

- **source code**: https://github.com/fortalbrz/doorbell

Once the pressing of the push button is detected by the NodeMCU, it publishes a message via the [MQTT]((https://www.home-assistant.io/integrations/mqtt/)) protocol to the [Home Assistant](https://home-assistant.io), which in turn plays voice messages on all compatible devices in the house, in addition to allowing several other types of integration.

[Home Assistant](https://home-assistant.io) is free and open-source software used for home automation. It serves as an integration platform and smart home hub, allowing users to control smart home devices. The MQTT (aka MQ Telemetry Transport) is a machine-to-machine or "*Internet of Things*" connectivity protocol on top of TCP/IP. It allows extremely lightweight publish/subscribe messaging transport.

## Features:
 - This "NodeMCU doorbell" can be connected in parallel to the push button of an alread installed doorbell.
 - The doorbell can play several tunes like star wars theme, super mario bros, etc.
 - The doorbell has 4 relays that can be used for: opens a front door (pulsed 12vdc), turns on external lights, etc
 - An internal "open front door" push button is also provided (this function can be enabled or disabled MQTT commands)
 - If the intenal "open front door" push button is pressed when it's disabled, and MQTT "alarm" notification is send (optional)!




## MQTT broker

This project should communicate with a MQTT broker (e.g., *mosquitto broker*), ideally using **[Home Assistant](https://home-assistant.io)**

![mqtt diagram](https://github.com/fortalbrz/gardenino/blob/main/img/schema.png?raw=true)


## Materials:
 - 1 x NodeMCU (ESP 8266-12e) - [[25 BRL](https://produto.mercadolivre.com.br/MLB-1211973212-modulo-wifi-esp8266-nodemcu-esp-12e-_JM)]
 - 1 x relay module 5v 4-ch - [[24 BRL](https://produto.mercadolivre.com.br/MLB-1758894951-modulo-rele-rele-5v-4-canais-para-arduino-pic-raspberry-pi-_JM)]
 - 1 x power supply 5vdc (1A) - [[14 BRL](https://produto.mercadolivre.com.br/MLB-3445635491-fonte-alimentaco-5v-1a-bivolt-roteador-wireles-modem-d-link-_JM)]
 - 1 x active buzzer 5v - [[2.30 BRL](https://www.a2robotics.com.br/buzzer-ativo-5v)]
 - 1 x NPN BC548 transistor - [[1.20  BRL](https://produto.mercadolivre.com.br/MLB-1712833525-transistor-bc548-npn-para-projetos-10-pecas-_JM )]
 - 1 x tactile push button - [[0.20 BRL](https://www.a2robotics.com.br/chave-tactil-6x6x5mm-4-terminais)]
 - 3 x resistor 10k ohms (1/8w) 
 - 1 x resistor 1k ohms (1/8w) 
 - 1 x led and resistor 10k ohms (optional, indicates "power on")
 - flexible cab (22 agw)

**Remark**: for using a 12 vdc pulsed open door locker, it's recommended to use a 12vdc power supply (1A) instead of the 5vdc as listed above. In this scenario, the NodeMCU could be powered by a 7802 voltage regulator, and additional components are required: 

- 7802 voltage regulator - [[2.50 BRL](https://www.robocore.net/regulador-de-tensao/regulador-de-tensao-5v-7805)] (optional)
- 2 x electrolytic capacitor 100 uF (optional)
## Circuit Wiring Instruction (step by step):


![project resources](https://github.com/fortalbrz/aquarioino/blob/main/bettaino/img/wiring_diagram.png?raw=true)

[[wiring diagram](https://www.circuito.io/app?components=513,13322,360216,442979)]:
- 7802 voltage regulator pin 1 (left pin) -> +12 Vdc power source (VCC) (optional for use a 12 Vdc power source)
- 7802 voltage regulator pin 1 (left pin) -> capacitor 100uF "A" (positive) (optional for use a 12Vdc power source)
- capacitor 100uF "A" (negative/"minus sign") -> -12 Vdc power source (GND) (optional for use a 12 Vdc power source)
- 7802 voltage regulator pin 2 (middle pin) -> -12 Vdc power source (GND) (optional for use a 12 Vdc power source)
- 7802 voltage regulator pin 3 (right pin: 5 Vdc regulated voltage) -> capacitor 100uF "B" (positive) (optional for use a 12 Vdc power source)
- capacitor 100uF "B" (negative/"minus sign") -> -12 Vdc power source (GND) (optional for use a 12 Vdc power source)
- NodeMCU "Vin" -> +5 Vdc power source (VCC) or 7802 voltage regulator pin 3 (right pin - 5 Vdc regulated voltage)
- NodeMCU "Gnd" -> power source (GND) (-5 Vdc or -12 Vdc)
- NodeMCU "D1" -> resistor 10k olhms "A" terminal 1
- resistor 10k olhms "A" terminal 2 -->  +5 Vdc power source (VCC) or 7802 voltage regulator pin 3 (right pin - 5 Vdc regulated voltage) 
- Arduino Nano "D1" -> tactile push buttom "DOORBELL" terminal 1 (NC)
- tactile push buttom "DOORBELL" terminal 2 (NC) --> power source (GND) (-5 Vdc or -12 Vdc)
- Relay 2 ch "VCC" +5 Vdc power source (VCC) or 7802 voltage regulator pin 3 (right pin - 5 Vdc regulated voltage)
- Relay 2 ch "GND" -> power source (GND) (-5 Vdc or -12 Vdc)
- NodeMCU "D2" -> Relay 4 ch "IN 1" (Door Locker)
- NodeMCU "D5" -> Relay 4 ch "IN 2" (Front Light)
- NodeMCU "D6" -> Relay 4 ch "IN 3" (general purpose)
- NodeMCU "D4" -> Relay 4 ch "IN 4" (general purpose)
- NodeMCU "D7" -> resistor 1k olhms "B" terminal 1
- resistor 1k olhms "B" terminal 2 -> transistor BC548 base (pin 2 - middle)
- transistor BC548 emitter (pin 3 - right) --> power source (GND) (-5 Vdc or -12 Vdc)
- transistor BC548 base (pin 1 - left) -> buzzer terminal 1 (negative)
- buzzer terminal 2 (positive) -> +5 Vdc power source (VCC) or 7802 voltage regulator pin 3 (right pin - 5 Vdc regulated voltage)
- Led terminal 1 (positive) --> +5 Vdc power source (VCC) or 7802 voltage regulator pin 3 (right pin - 5 Vdc regulated voltage) (optional, "power on led")
- Led terminal 2 (negative/bevel) --> resistor 10k olhms "C" terminal 1 (optional, "power on led")
- resistor 10k olhms "C" terminal 2 --> power source (GND) (-5 Vdc or -12 Vdc) (optional, "power on led")


### Remarks:
 - NodeMCU pins D3 and D4 must be pull-up: boot fails on low, thus this are required high on boot.
 -  NodeMCU pin D4 is connected to build-in LED: do not use BUILTIN led.

## Source code:
 - **https://github.com/fortalbrz/doorbell**


## Flashing the code

**Drivers (CH340g)** for NodeMCU:
  - [CH340g USB/Serial driver](https://bit.ly/44WdzVF) (windows 11 compatible driver)  
  - driver install instructions ([pt](https://bit.ly/3ZqIqc0))

The ESP-01 module should be programed with the sketch with the [Arduino IDE](https://www.arduino.cc/en/software):
  - go to File > Preferences
  - on "Additional boards manager", set the value "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
  - go to Tools > Board > Board Manager
  - search for "**ESP8266**"
  - install the ESP8266 Community package ("**esp8266**" by *ESP8266 Community*)//   
  - select board "**NodeMCU 1.0 (ESP-12E Module)**" and connected COM port (checks at Windows "device manager")
  - select Sketch > Upload


### Error Codes (buzzer)
   - 2 fast bips: Wifi error
   - 3 fast bips: MQTT broker error


### Wiring Testing:

Sets the macro "**WIRING_TEST_MODE**" as true in order to check buttons, relays and water sensor connections (testing only)

### Serial Monitor:
Sets the macro "**DEBUG_MODE**" as true in order to debug on serial monitor (testing only) - ensure the baud rate setup!


## Home Assistant Configuration

Adds the line on *configuration.yaml*: 


     mqtt: !include mqtt.yaml


And creates a file "*[mqtt.yaml](https://github.com/fortalbrz/doorbell/blob/main/mqtt.yaml)*" as follow:


     - binary_sensor:
        #
        # Doorbell state: on/off
        #
        name: "Doorbell Ringing"
        state_topic: "doorbell/ring"
        payload_on: "on"
        payload_off: "off"
        availability:
          - topic: "doorbell/available"
            payload_available: "online"
            payload_not_available: "offline"
        qos: 0
        device_class: "motion"
        icon: mdi:doorbell_video
    - switch:
        #
        # Front door lights: on/off
        #
        name: "Front Door Lights"
        unique_id: front_door_lights
        state_topic: "doorbell/status"
        value_template: "{{ value_json.light }}"
        state_on: "on"
        state_off: "off"
        command_topic: "doorbell/cmd"
        payload_on: "light on"
        payload_off: "light off"
        availability:
          - topic: "doorbell/available"
            payload_available: "online"
            payload_not_available: "offline"
        availability_mode: latest
        enabled_by_default: true
        optimistic: false
        qos: 0
        retain: true
        icon: mdi:light-flood-down
        device_class: "outlet"
    - switch:
        #
        # Enables opening the front door on physical button: on/off
        #
        name: "Enables Opening Front Door"
        unique_id: front_door_can_open
        state_topic: "doorbell/status"
        value_template: "{{ value_json.enabled }}"
        state_on: "on"
        state_off: "off"
        command_topic: "doorbell/cmd"
        payload_on: "open door on"
        payload_off: "open door off"
        availability:
          - topic: "doorbell/available"
            payload_available: "online"
            payload_not_available: "offline"
        availability_mode: latest
        enabled_by_default: true
        optimistic: false
        qos: 0
        retain: true
        icon: mdi:door-closed-lock
        device_class: "switch"
    - button:
        #
        # opens the front door
        #
        name: "Opens Front Door"
        unique_id: front_door_open
        command_topic: "doorbell/cmd"
        payload_press: "open door"
        availability:
          - topic: "doorbell/available"
            payload_available: "online"
            payload_not_available: "offline"
        qos: 0
        retain: false
        entity_category: "config"
        device_class: restart




## MQTT topics:

### state topics:

   - **doorbell/available**: sensors availability [*"online"/"offline"*]

   - **doorbell/ring**: "on" for doorbell ringing [*"on"/"off"*]

   - **doorbell/state**: retrieves NodeMCU states as json [*doorbeel -> home assistant*]


         {
              "light": "on/off",        // light state (relay #02) 
              "relay3": "on/off",       // relay #03 state
              "relay4": "on/off",       // relay #03 state
              "enabled": "on/off",      // enables/disables physical open door button
              "running": "on/off"       // running state
         } 

   - **doorbell/alarm**: "on" for alarm status (i.e., the physical "open front door" button was pressed when it was disabled)

   
### command topics:

  - **doorbell/cmd**: pushes commands to NodeMCU [*home assistant -> doorbell*]:
    - "*open door*": opens the front door (relay #01: pulsed)
    - "*light on/off*": turn on/off the external lights (relay #02)
    - "*relay3 on/off*": turn on/off the relay #03
    - "*relay4 on/off*": turn on/off the relay #04
    - "*open door on/off*": enables/disables physical fort door open button
      - "*play [sw/dv/tetris/mario/got/gf/nokia/notice]*": plays an tune by name [for any notification] (*none for random*) [debug only]
    - "*ring*": simulates a doorbell push button press [debug only]
    - "*refresh*": update MQTT state [debug only]



### Configuration flags
  

| macro                      | default | description                                                                                   |
|----------------------------|---------|-----------------------------------------------------------------------------------------------|
| WIFI_SSID                  |         | Wi-fi SSID                                                                                    |
| WIFI_PASSWORD              |         | Wi-fi password                                                                                |
| MQTT_BROKER_ADDRESS MQTT   |         | MQTT broker server ip address                                                                 |
| MQTT_BROKER_PORT           | 1883    | MQTT broker port                                                                              |
| MQTT_USERNAME              |         | MQTT broker username                                                                          |
| MQTT_PASSWORD              |         | MQTT broker password                                                                          |
| MQTT_DEVICE_ID             |         | MQTT session identifier (changes for more then one gardeino on the same MQTT broker)          |
| DEBUG_MODE                 | false   | true to debug on serial monitor (debug), false otherwise                                      |
| ENABLE_WATER_REPOSITION    | true    | enables/disables water level sensors (disable it to not use the water level sensors)          |
| USE_BUZZER                 | true    | enables/disables buzzer code (disable to not use a buzzer)                                    |
| PLAY_TUNES                 | true    | enables play music tunes (requires a buzzer)                                                  |
| DOORBELL_TUNE              | -1      | doorbell tune: -1: random, 0: star wars, 1: darth vader, 2: tetris, 3: super mario bros, etc  |
| WIRING_TEST_MODE           | false   | enables/disables a wiring test routine                                                        |


<hr>

*[Jorge Albuquerque](https://linkedin.com/in/jorgealbuquerque) (2024)*
