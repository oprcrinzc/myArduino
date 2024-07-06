#include<SoftwareSerial.h>
SoftwareSerial mySerial(6, 7); // txd, rxd

int iA1 = 4, iA2 = 2, pwmA = 3;
int iB1 = 13, iB2 = 12, pwmB = 11; 

int outp[] = {iA1, iA2, pwmA, iB1, iB2, pwmB}; 
void dw(int p, int s){ digitalWrite(p, s);}

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

class Motor{
  public:
    int pin1, pin2, pwm, pole = 0;
    Motor(int i1, int i2, int ipwm){
      pin1=i1;
      pin2=i2;
      pwm=ipwm;
      }
    void polaris(int ipole){
      if(ipole == -1 || ipole == 0 || ipole == 1){
        if (ipole == -1) {
            if(pole == 0) {
                polaris(1);
            } else if(pole == 1) {
                polaris(0);
            }
        } else if (ipole == 0) {
            dw(pin1, LOW);
            dw(pin2, HIGH);pole = ipole;
        } else if (ipole == 1) {
            dw(pin1, HIGH);
            dw(pin2, LOW);pole = ipole;
        }

      }
    }
    void forward(int speed){
      polaris(0);delay(10);
      Serial.println("forward");
      analogWrite(pwm, speed);
    }
    void backward(int speed){
      polaris(1);delay(10);
      Serial.println("backward");
      analogWrite(pwm, speed);
    }
    void go(int speed){
      // polaris(1);delay(10);
      Serial.println("go");
      analogWrite(pwm, speed);
    }
};
Motor A(iA1, iA2, pwmA);
Motor B(iB1, iB2, pwmB);

void Forward(){
  A.forward(255);
  B.forward(255);
}
void Backward(){
  A.backward(255);
  B.backward(255);
}
void TurnLeft(){
  A.forward(0);
  B.forward(200);
}
void TurnRight(){
  A.forward(200);
  B.forward(0);
}
void Stop(){
  A.go(0);
  B.go(0);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  for(int i = 0; i < sizeof(outp)/sizeof(outp[0]); i++){
    pinMode(outp[i], OUTPUT);
    // Serial.println(outp[i]);
  }
  A.polaris(0);
  B.polaris(1);
  
}

void loop() {
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
    else if (cmd==0b110100)                                                               Stop();
    else if (cmd==0b110011)                                                               Forward();
    else if (cmd==0b110010)                                                               Backward();
    else if (cmd==0b110001)                                                               TurnLeft();
    else if (cmd==0b110000)                                                               TurnRight();
    Serial.println(cmd, BIN);
  }
}

