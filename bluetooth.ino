// Writen by OPRC, oprcrinzc

#include<Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // txd, rxd
Servo servo1;

byte cmd;
byte stack0; // a, z command
byte stack1; // space
byte stack2; // command parameter number 0, 9
byte stack3; // space
byte stack4; // number 0, 9
byte stack5; // number 0, 9
byte stack6; // number 0, 9
int stack_number_count = 4; // 4, 6
int stack_number_write = 0;

int led1_pin = 12;
int led2_pin = 11;

void setup() {
  servo1.attach(13); // orange wire
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
}

void toggleLed(int pin){
  int debug = 0;
  int state = digitalRead(pin);
  if(debug==1){
    Serial.println("____________");
    Serial.print("pin: ");
    Serial.print(pin);
    Serial.print("\n");
    Serial.print("state: ");
    Serial.print(state);
    Serial.println("____________");
  }
  if(state==0) digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);
}

int binTextToInt(byte bin){
  if      (bin==0b110000) return 0;
  else if (bin==0b110001) return 1;
  else if (bin==0b110010) return 2;
  else if (bin==0b110011) return 3;
  else if (bin==0b110100) return 4;
  else if (bin==0b110101) return 5;
  else if (bin==0b110110) return 6;
  else if (bin==0b110111) return 7;
  else if (bin==0b111000) return 8;
  else if (bin==0b111001) return 9;
  else                    return -1;
};

void loop() {
  // servo1.write(10);
  if(mySerial.available()){
    cmd = mySerial.read();
    if (stack0==0b1110011 && stack1==0b100000 && stack2==0b110001 && stack3==0b100000){
      int debug = 0;
      if(stack_number_write < 3){
        if(debug){
          Serial.println("________");
          Serial.println(stack_number_count);
          Serial.println(stack4);
          Serial.println(stack5);
          Serial.println(stack6);
          Serial.println("________");
        }
        if(stack_number_count==4) stack4 = cmd;
        if(stack_number_count==5) stack5 = cmd;
        if(stack_number_count==6) stack6 = cmd;
        stack_number_count += 1;
        stack_number_write += 1;
      }
      
    }
    else if (cmd==0b1010);
    else if (cmd==0b1101);
    else if (stack0==0b1110011 && stack1==0b100000 && stack2==0b110001 && cmd==0b100000)  stack3 = 0b100000;
    else if (stack0==0b1110011 && stack1==0b100000 && cmd==0b110001)                      stack2 = 0b110001;
    else if (stack0==0b1110011 && cmd==0b100000)                                          stack1 = 0b100000;
    else if (cmd==0b1110011)                                                              stack0 = 0b1110011;
    else if (cmd==0b110011)                                                               toggleLed(led2_pin);
    else if (cmd==0b110010)                                                               toggleLed(led1_pin);
    else if (cmd==0b110001)                                                               servo1.write(180);
    else if (cmd==0b110000)                                                               servo1.write(0);
    Serial.println(cmd, BIN);
    // Serial.println(stack4, BIN);
    // Serial.println(stack5, BIN);
    // Serial.println(stack6, BIN);
    // Serial.println(binTextToInt(cmd));
  }
  if (stack0==0b1110011 && stack1==0b100000 && stack2==0b110001 && stack3==0b100000 && stack4  && stack5  && stack6 ){
    int hun = binTextToInt(stack4);
    int ten = binTextToInt(stack5);
    int dig = binTextToInt(stack6);
    int deg = (hun*100)+(ten*10)+(dig);
    Serial.println("DEGGG");
    servo1.write(deg);
    stack0=NULL;
    stack1=NULL;
    stack2=NULL;
    stack3=NULL;
    stack4=NULL;
    stack5=NULL;
    stack6=NULL;
    stack_number_count = 4;
    stack_number_write = 0;
  }
}
