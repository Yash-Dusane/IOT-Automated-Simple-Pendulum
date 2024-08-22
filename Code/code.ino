#define SWITCH_VIRTUAL_PIN0 V0
#define SWITCH_VIRTUAL_PIN4 V4
#define Servo_PWM 5
#define Servo_PWM2 12
#define BLYNK_TEMPLATE_ID "TMPL3cXll1qNU"
#define BLYNK_TEMPLATE_NAME "ESP32SimplePendulum"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "ESP32Servo.h"

char ssid[] = "Galaxy S21 FE 5Gbdae";
char pass[] = "xndw9387";
// char ssid[] = "rohan's iPhone";
// char pass[] = "Rohan2004";
char auth[] = "0vn1VZS0gh1HvpWuagscXe5aw5Qbpslf";
Servo MG995_Servo;

int flag = 2;
int counter;
double sumtime;
int expcount = 0;
int halftime = 0;
const int irSensor = 14;
unsigned long starttime;
unsigned long currtime = 0;
unsigned long Elapsedtime;
int flag1 = 0;
int state = 0;
int irReading;
double currlength = 0;
double string_length = 0;
double prev_length = 0;
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  

  Blynk.run();
  if (currlength != prev_length) {
    if (currlength == 1 and prev_length == 0) {
      Serial.println("To the top, from the middle");
      MG995_Servo.attach(Servo_PWM2);
      MG995_Servo.write(83);
       delay(5000);
      MG995_Servo.detach();
    }
    else if(currlength == 0 and prev_length == 1) {
      Serial.println("Downward motion,from the top");
      MG995_Servo.attach(Servo_PWM2);
      MG995_Servo.write(101);
       delay(4400);
      MG995_Servo.detach();
    }
    else if(currlength == -1 and prev_length == 0) {
      Serial.println("Downward motion,from the middle");
      MG995_Servo.attach(Servo_PWM2);
      MG995_Servo.write(101);
       delay(2800);
      MG995_Servo.detach();
    }
    else if(currlength == 0 and prev_length == -1) {
      Serial.println("Upward motion, back to the middle");
      MG995_Servo.attach(Servo_PWM2);
      MG995_Servo.write(83);
       delay(3250);
      MG995_Servo.detach();
    }
    // Serial.println(currlength);
    // Serial.println(prev_length);

    prev_length=currlength;
  }
  Serial.println("Length has been recorded!!!!!!!!!!!!!!!");
  if(currlength==1){
    string_length = 0.295;
  }
  else if(currlength == 0){
    string_length = 0.195;
  }
  else if(currlength == -1){
    string_length = 0.125;
  }

  if (state == 1 and expcount == 1) { state = 0; }
  if (flag == 1) { state = 1; }
  if (state == 0) { expcount = 0; }

  if (state == 1) {

    if (flag == 2) {
      flag = 1;

      MG995_Servo.attach(Servo_PWM);
      MG995_Servo.write(104);
      delay(5300);
      MG995_Servo.detach();
      delay(1000);
      MG995_Servo.attach(Servo_PWM);
      MG995_Servo.write(81);
      delay(5700);
      MG995_Servo.detach();

      counter = 0;
      sumtime = 0;

      currtime = millis();
      //starttime = millis();
      Elapsedtime = 0;
      Serial.println(currtime);
      Serial.println("THE VALUE OF flag IS 1!");
    }

    if (millis() - currtime >= 10 && flag == 1 && counter <= 20) {
      //Serial.println("IR SENSOR NOW READING!");
      //delay(100);
      irReading = digitalRead(irSensor);

      if (irReading == LOW) {
        if (flag1) {
          if (counter > 0) {
            Elapsedtime = millis() - starttime;
            if (counter % 2 == 1) { halftime = Elapsedtime; }
            if (counter % 2 == 0) {
              Blynk.virtualWrite(V2, (sumtime/10000));
            }
          }
          flag1 = 0;
          starttime = millis();
          counter++;
          sumtime += Elapsedtime;
        }
        //Serial.println(counter);
      } else {

        flag1 = 1;
        //Serial.println("THE VALUE OF flag1 IS 1!");
      }
      currtime = millis();
    }
    //Serial.println(counter);
    if (counter > 20) {


      Blynk.virtualWrite(V1, ((4 * 3.14 * 3.14 * string_length * 10000 * 10000) / (sumtime * sumtime)));
      flag = 2;
      expcount = 1;
      Blynk.virtualWrite(V0, 0);
      Blynk.virtualWrite(V3, sumtime / 10000);
      Serial.println(sumtime / 10000);
    }
  }
}

BLYNK_WRITE(SWITCH_VIRTUAL_PIN0) {
  state = param.asInt();
  Serial.println("WE ARE ONLINE!!");
}

BLYNK_WRITE(SWITCH_VIRTUAL_PIN4) {
  currlength = param.asInt();
  Serial.println("The length has been taken, thank you very much!");
}