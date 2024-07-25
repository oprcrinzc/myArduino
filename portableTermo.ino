#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <sstream>
#include <DHT.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

DHT dht(19, DHT22);
int munit = 0; 

std::vector<int> btn({34});
std::vector<int> btn_last_state(btn.size());
std::vector<int> btn_current_state(btn.size());

int playMode = 0;
int lastPlayMode = 0;

void updateBtnState(int mode=0){
  if(mode==0){
    for(int i=0;i< btn.size();i++){
      btn_current_state[i] = digitalRead(btn[i]);
    }
  } else if(mode == 1){
    btn_last_state = btn_current_state;
  }
}

void toggleCMMM(){
  munit += 1;
  if(munit>2) munit = 0;
}


void setup() {
  Serial.begin(9600);

  for(int pin : btn) pinMode(pin, INPUT_PULLUP);
  updateBtnState(0);
  updateBtnState(1);

  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
}

void loop() {
  updateBtnState(0);
  int btn_num = 0;
  for(int state : btn_current_state){
    if(btn_num==0 && state==1 && btn_last_state.at(btn_num) == 0) {
      toggleCMMM();
      Serial.println(state);
      }
    btn_num+=1;
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float show;
  std::string units;
  if(munit==0){units="C";show=t;}
  else if(munit==1){units="F";show=f;}
  else if(munit==2){units="%";show=h;}
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(show);
  display.setCursor(100,15);
  display.setTextSize(2);
  display.print(units.c_str());
  display.display();
  updateBtnState(1);
}
