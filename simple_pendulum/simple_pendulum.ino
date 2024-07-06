// Include necessary libraries
#include <WiFi.h>
#include <HTTPClient.h>


#include "ESP32Servo.h" // include servo library to use its related functions
#define Servo_PWM 5 // A descriptive name for D6 pin of Arduino to provide PWM signal
Servo MG995_Servo;  // Define an instance of of Servo with the name of "MG995_Servo"

  
// Define constants
const char* ssid = "Galaxy S21 FE 5Gbdae";
const char* password = "xndw9387";
const char* server = "api.thingspeak.com";
const String readAPIKey = "RXS9UOYEAB9H54HP";

//String fieldValue = "0";
int flag = 0;
int counter;


const int irSensor = 14;
//const int ledPin = 32;
unsigned long starttime;
unsigned long currtime = 0;
unsigned long Elapsedtime;
int flag1 = 0;
int irReading;
//const int motorPin = 12; // Define the pin number where the motor is connected



void setup() {

  
  // Initialize serial communication
  Serial.begin(115200);
  
  pinMode(irSensor, INPUT);
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

}



void loop() {

  String fieldValue = readThingSpeak();

   if(fieldValue == "0")
   {flag = 0;}
  
  if (fieldValue == "1" && flag == 0) {

    MG995_Servo.attach(Servo_PWM); 
    MG995_Servo.write(104);
    //delay(4000);
    MG995_Servo.detach();//Stop.
    counter = 0; 
    currtime = millis();
    flag = 1;  
  }

  if(millis() - currtime >= 1 && flag == 1 && counter <= 30) {
      irReading = digitalRead(irSensor);
    if (irReading == LOW )
    {
  //    Serial.println("0");
        if(flag1) {
          Elapsedtime = millis() - starttime;
          flag1 = 0;
          starttime = millis();
          counter++;
          Serial.println(Elapsedtime);
        } 
       Serial.println();
    }
    else
    {
  //    Serial.println("1");
        flag1 = 1;
    }
    currtime = millis();
  }
  
  if(counter > 30){
    
    
      MG995_Servo.attach(Servo_PWM);
      MG995_Servo.write(81);
     // delay(4000);
      MG995_Servo.detach();//Stop
     // delay(1000);

  }

  
}



String readThingSpeak() {
  
  // Create HTTPClient object
  HTTPClient http;
  // Construct URL for ThingSpeak read API
  String url = "http://api.thingspeak.com/channels/2504551/fields/1/last?api_key=" + readAPIKey;
 
  // Send GET request to ThingSpeak
  http.begin(url);
  int httpResponseCode = http.GET();

  String fieldValue = "";
  // If request was successful, read response
  if (httpResponseCode == 200) {
    fieldValue = http.getString();
    Serial.print("Field Value from ThingSpeak: ");
    Serial.println(fieldValue);
  } else {
//    Serial.print("Error reading from ThingSpeak. HTTP Error code: ");
//    Serial.println(httpResponseCode);
  }

  // Close connection
  http.end();
  
  return fieldValue;
}
