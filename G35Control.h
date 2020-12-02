#include "esphome.h"
#include "G35String.h"
#include "G35.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/light/addressable_light.h"


uint8_t LIGHT_COUNT =36;

// Arduino pin number. Pin 13 will blink the on-board LED.
uint8_t G35_PIN =23;

int LOOPSPEED = 25;

G35String lights(G35_PIN, LIGHT_COUNT);


int mode = 0;
 static const uint8_t TAIL = 5;

  uint8_t d_;
  int16_t position_;
  color_t colors_[TAIL];
 
 
class XmasLightsLight : public Component, public LightOutput {
 public:
 uint8_t step =0;
 int loopcount = 0;
  void setup() override {
    // This will be called by App.setup()
    pinMode(5, INPUT);
    
    lights.enumerate();
        
        lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_ORANGE);
      delay(1000);
      lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_VIOLET);
      delay(1000);
      lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_CYAN);
      delay(1000);
      lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
      delay(1000);
    
    
  }
  LightTraits get_traits() override {
    // return the traits this light supports
    auto traits = LightTraits();
    traits.set_supports_brightness(true);
    traits.set_supports_rgb(true);
    traits.set_supports_rgb_white_value(false);
    traits.set_supports_color_temperature(false);
    return traits;
  }
 
  void write_state(LightState *state) override {
    // This will be called by the light to get a new state to be written.
    float red, green, blue;
    // use any of the provided current_values methods
    state->current_values_as_rgb(&red, &green, &blue);
    // Write red, green and blue to HW
    bool OnOffState;
    float brightness;
    state->current_values_as_brightness(&brightness);
    state->current_values_as_binary(&OnOffState);
    
    uint8_t bright;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    
      bright = map(brightness*1000,0,1000,0,G35::MAX_INTENSITY);
      r = map(red*1000,0,1000,0,15); // map colors to 4 bit color depth
      g = map(green*1000,0,1000,0,15);
      b = map(blue*1000,0,1000,0,15);
    
    if (OnOffState){
          lights.fill_color(0, LIGHT_COUNT, bright, COLOR(r, g, b));
          
           ESP_LOGD("custom", "LightState State on");
    }else{
        mode=0;
       lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
        ESP_LOGD("custom", "LightState State off ");
    } 
     
 
  }
  
  // use this loop to handle the modes for the light
  void loop() override{

      // only process light updates each LOOPSPEED times through the loop.
      
      if (loopcount>LOOPSPEED){ 
        if(mode ==1){
            if (step==1){
   
               lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);
               step=0;
            }else{
              lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);
              step =1;
            }

        }else if (mode==2){
          for (uint8_t i=0;i<LIGHT_COUNT;i++){
            if (step==1){
              lights.set_color_if_in_range(i, G35::MAX_INTENSITY, COLOR_RED);
            }else{
              lights.set_color_if_in_range(i, G35::MAX_INTENSITY,COLOR_GREEN);
            }
            if (step==0){
              step =1;
            }else{
              step=0;
            }
                                      
          }
           if (step==0){
              step =1;
            }else{
              step=0;
            }

        }else if (mode==3){
             colors_[0] = COLOR_GREEN;
              colors_[1] = COLOR_RED;
              colors_[2] = COLOR_WARMWHITE;
              colors_[3] = COLOR_BLACK;
              colors_[4] = COLOR_BLACK;
               

              for (int i = 0; i < LIGHT_COUNT; ++i) {
                if(step>=5)step-=5;
                lights.set_color_if_in_range(i, G35::MAX_INTENSITY, colors_[step]);
                step++;
                

              }
              //step++;

        }else if (mode==4){
             colors_[0] = COLOR_GREEN;
              colors_[1] = COLOR_RED;
              colors_[2] = COLOR_WARMWHITE;
              colors_[3] = COLOR_BLACK;
              colors_[4] = COLOR_BLACK;
               

              for (int i = 0; i < LIGHT_COUNT/2; ++i) {
                if(step>=5)step-=5;
                lights.set_color_if_in_range(i, G35::MAX_INTENSITY, colors_[step]);
                lights.set_color_if_in_range(LIGHT_COUNT-i, G35::MAX_INTENSITY, colors_[step]);
                step++;
                

              }
              step++;

        }
        
        loopcount =0;
      }else{
        loopcount++;
      }
        delay(10); // delay for each time through the loop to slow down the light refresh speed.
    }
    protected: 
    
    float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
    {
     return float(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
    }
};


class XMASLights_RedGreenSwap:   public Component , public Switch{
    public:  
    void write_state(bool state) override {
       if (state){
           mode =1;
           ESP_LOGD("custom", "RedGreen State on");
       }else{
           mode=0;
           ESP_LOGD("custom", "RedGreen State off");
           lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
       }
        publish_state(state);
    }
}; 

class XMASLights_RedGreenAlternate:   public Component , public Switch{
    public:  
    void write_state(bool state) override {
       if (state){
           mode =2;
           ESP_LOGD("custom", "RedGreen State on");
       }else{
           mode=0;
           ESP_LOGD("custom", "RedGreen State off");
           lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
       }
        publish_state(state);
    }
}; 

class XMASLights_Chase:   public Component , public Switch{
    public:  
    void write_state(bool state) override {
       if (state){
           mode =3;
           ESP_LOGD("custom", "Chase State on");
       }else{
           mode=0;
           ESP_LOGD("custom", "Chase State off");
           lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
       }
        publish_state(state);
    }
}; 
 
class XMASLights_MiddleIn:   public Component , public Switch{
    public:  
    void write_state(bool state) override {
       if (state){
           mode =4;
           ESP_LOGD("custom", "Chase State on");
       }else{
           mode=0;
           ESP_LOGD("custom", "Chase State off");
           lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
       }
        publish_state(state);
    }
}; 