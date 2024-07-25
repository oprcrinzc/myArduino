#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <sstream>
#include <HCSR04.h>
// #include <DHT22.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

UltraSonicDistanceSensor ultraSo(26, 25);
double distance;
int munit = 0; 

std::vector<int> btn({36, 39, 34, 35});
std::vector<int> btn_last_state(btn.size());
std::vector<int> btn_current_state(btn.size());

int playMode = 0;
int lastPlayMode = 0;



// for graph
int width = 62;
int height = 16;
int r = 2;
int vx = -width+r;
int vy = height-r;
int xind = 1;
int yind = -1;
int st = r;
int dp_h, dp_w;

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
  Serial.begin(300);

  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  for(int pin : btn) pinMode(pin, INPUT_PULLUP);
  updateBtnState(0);
  updateBtnState(1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
}

void loop() {
  updateBtnState();
  int btn_num = 0;
  for(int state : btn_current_state){
    // if(i==0 && state==1 && btn_last_state.at(i) == 0) test();
    if(btn_num==0 && state==1 && btn_last_state.at(btn_num) == 0 && playMode==0) toggleCMMM();
    else if(btn_num==1 && state==1 && btn_last_state.at(btn_num) == 0) playMode=1;
    else if(btn_num==2 && state==1 && btn_last_state.at(btn_num) == 0) playMode=0;
    else if(btn_num==3 && state==1 && btn_last_state.at(btn_num) == 0 && playMode==1) {if(xind==1){xind=-1;}else{xind=1;}}
    else if(btn_num==0 && state==1 && btn_last_state.at(btn_num) == 0 && playMode==1) {if(yind==1){yind=-1;}else{yind=1;}}
    btn_num+=1;
  }

  if(playMode==0){
    distance = ultraSo.measureDistanceCm();
    std::string units;
    if(munit==0) {units="M";distance /= 100;}
    else if(munit==1) units="CM";
    else if(munit==2) {units="MM";distance *= 10;}
    
    // Serial.println(distance);
    // if(distance < 0) return;
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    // display.println("OPRC");
    display.setCursor(0, 0);
    display.print(distance);
    display.setCursor(100,15);
    display.setTextSize(2);
    display.print(units.c_str());
    display.display();
  } else if(playMode==1){
    display.clearDisplay();
    // display.setCursor(0, 0);
    // display.setTextColor(WHITE);
    // // display.setTextSize(1);
    // display.printf("xind : ");
    // display.print(xind);
    // display.println("xind : " + std::to_string(xind));
    if(vx>=width-r) xind = -1;
		else if(vx<=-width+r+1) xind = 1;
		if(vy>=height-r) yind = -1;
		else if(vy<=-height+r) yind = 1;
		for(int i=height;i>-height;i--){
			for(int j=-width;j<width;j++){
        dp_w = j + width;
        dp_h = height - i;
        if(j==0 || i==0) display.drawPixel(dp_w, dp_h, WHITE);
        // if(i >= 8) display.drawPixel(dp_w, dp_h, WHITE);
        // if(i <= -5) display.drawPixel(dp_w, dp_h, WHITE);
				if(r*r == (j-vx)*(j-vx) + (i-vy)*(i-vy)) {
          // display.drawPixel(dp_w, dp_h, WHITE);
          display.fillCircle(dp_w, dp_h, 1, WHITE);
          // if(i >= 8 || i <= -5) dead();
          }
				// else cout << " ";
			}
      
			// cout << "| ";
			// if(i==height) {cout << " " << xind << " " << yind;}
			// display.display();
		}
		vx += xind;
		vy += yind;
    display.display();
  }
  
  updateBtnState(1);
}

void dead(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.println("DIE");
}

void fullWhite(){
  display.clearDisplay();
  for(int i=0; i < 32; i++){
    for(int j=0;j < 124; j++){display.drawPixel(j, i, WHITE);}
  }
  display.display();
}

void test(){
  display.clearDisplay();
  for(int i=0; i < 32; i++){
    for(int j=0;j < 124; j++){
      if(i-4%2 == 0 && j-4%2 == 0){ display.drawPixel(j, i, WHITE);
      } else if (i-4%2 == 1 && j-4%2 == 1) display.drawPixel(j, i, WHITE);
    }
    display.display();
  }
  display.clearDisplay();
  for(int i=0; i < 32; i++){
    for(int j=0;j < 124; j++){display.drawPixel(j, i, WHITE);}
    display.display();
  }
}