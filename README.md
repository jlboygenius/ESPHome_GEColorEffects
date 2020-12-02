# ESPHome_GEColorEffects
Control GE Color Effects Light Bulbs using an ESP32. Make them work with ESPHome and Home Assistant as a Light bulb and Switches for different modes. These lights only support 4 bit color per pixel.


Thanks to sowbug for putting together how to control these lights. https://github.com/sowbug/G35Arduino

PreReqs:
Home Assistant
ESPHome

1: Create a new device in ESPHome (g35_Christmas_lights).  Compile and upload to ESP with basic config. You'll need to do this to get ESPHome to create the folders for the source files.
2: Copy g35_christmas_lights.yaml into the ESP Device yaml that you created. Add in your WIFI AP and password. 
3: Load the G35SString.h, G35string.cpp, G35Control.h, G35.h, G35.cpp files into the \config\esphome\g35_christmas_lights\src directory in Home Assistant.
4: Upload to your ESP32.


Hardware Config:
You'll need to cut out the original controller for the light string. It cannot be used with this. 
The Power and Ground wires will stay the same, but the Data wire will connect to the ESP32.
The power supply lines are labled + and -. Connect the + to the VIN of the ESP32. Connect the - to the Ground of the ESP32.
Connect the Data wire (middle wire going to bulbs) to pin 23 on the ESP32. This is defined in G35Control.h


Once installed, this will create a switch called "Front Door XMAS Lights" (can be renamed in the YAML). This "Light" can be used to control the brightness and color of the string of lights. All lights will be the same color. Also supports the Random, Strobe, and Flicker effects. See ESPHome support for effect configurations. 

4 Switches are also defined. Turning these on will switch the light into different modes. These different modes are defined in the Loop() function of the light. 
* Xmas Lights Red Green Alternate - Enter string alternates between red and green
* Xmas Lights Red Green Swap - Every other light alternates between red and green
* Xmas Lights Red Green Chase - lights chase from beginning to end 
* Xmas Lights Red Green Middle In - lights chase from ends to center
Each switch will over-ride the previous one, but not turn it off. Best to turn off one switch before turning on the next.

More switches can be configured by copying the Class definition in the g35Control.h file. You'll also need to create a new lambda in the YAML to define the switch and create the instance. The switch simply sets the mode variable. This mode variable is used in the Light's loop function to define the lighting patterns.


TODO: 
* Expand so that Light inherits from the Adressable Light class. This would allow EspHome to see the light as an LED strip like NeoPixel or FastLed
* Add more switches to define more patterns
