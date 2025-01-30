//##################################################################################################################
//##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
//##                                                                                                              ##
//##################################################################################################################

// LIBRARY IMPORTS
#include <rom/rtc.h>

#ifndef _WIFI_H 
#include <WiFi.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 
 
#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif

// DEFINE VARIABLES
#define ARDUINOJSON_USE_DOUBLE      1 
// DEFINE THE PINS THAT WILL BE MAPPED TO THE 7 SEG DISPLAY BELOW, 'a' to 'g'
#define a     15
#define b     32
#define c     33
#define d     25
#define e     26
#define f     27
#define g     14
#define dp    12
/* Complete all others */



// DEFINE VARIABLES FOR TWO LEDs AND TWO BUTTONs. LED_A, LED_B, BTN_A , BTN_B
#define LED_A 4
#define LED_B 2
#define BTN_A 17
#define BTN_B 
/* Complete all others */



// MQTT CLIENT CONFIG  
static const char* pubtopic       = "620164419";                    // Add your ID number here
static const char* subtopic[]     = {"620164419_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server    = "www.yanacreations.com";                // Broker IP address or Domain name as a String 
static uint16_t mqtt_port         = 1883;

// WIFI CREDENTIALS
const char* ssid                  = "MonaConnect"; // Add your Wi-Fi ssid
const char* password              = ""; // Add your Wi-Fi password 




// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL; 

// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters ); 
void GDP(void);   // GENERATE DISPLAY PUBLISH

/* Declare your functions below */
void Display(unsigned char number);
int8_t getLEDStatus(int8_t LED);
void setLEDState(int8_t LED, int8_t state);
void toggleLED(int8_t LED);
  

//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif

// Temporary Variables
uint8_t number = 0;


void setup() {
 // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network"); 

  // CONFIGURE THE ARDUINO PINS OF THE 7SEG AS OUTPUT
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(dp,OUTPUT);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BTN_A, INPUT_PULLUP);
  //pinMode(BTN_B, INPUT_PULLUP);

  /* Configure all others here */
  Display(8);
  initialize();// INIT WIFI, MQTT & NTP 
  vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION

}
  


void loop() {
    // put your main code here, to run repeatedly: 
}




  
//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      for (;;){
        
        // Add code here to check if a button(S) is pressed
        // then execute appropriate function if a button is pressed
          if(digitalRead(BTN_A) == LOW) {
            GDP();
            delay(500); // debouncing button, 0.5s delay

          }
        //Check if button B is pressed
         // if (digitalRead(BTN_B) == LOW) {
          //  toggleLED(LED_B);
        }


        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }



void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
           
    for( ;; ) {
          // Task code goes here.   
          // PUBLISH to topic every second.
          JsonDocument doc; // Create JSon object
          char message[1100]  = {0};

          // Add key:value pairs to JSon object
          doc["id"]         = "620164419";

          serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array

          if(mqtt.connected() ){
            publish(pubtopic, message);
          }
          
            
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
}

unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO
  
  Serial.printf("\nMessage received : ( topic: %s ) \n",topic ); 
  char *received = new char[length + 1] {0}; 
  
  for (int i = 0; i < length; i++) { 
    received[i] = (char)payload[i];    
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n",received);

 
  // CONVERT MESSAGE TO JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, received);  

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  // PROCESS MESSAGE
  const char* type = doc["type"];

  if (strcmp(type, "toggle") == 0){
    // Process messages with ‘{"type": "toggle", "device": "LED A"}’ Schema
    const char* led = doc["device"];

    if(strcmp(led, "LED A") == 0){
      /*Add code to toggle LED A with appropriate function*/
      toggleLED(LED_A);
    }
    if(strcmp(led, "LED B") == 0){
      /*Add code to toggle LED B with appropriate function*/
      toggleLED(LED_B);
    }

    // PUBLISH UPDATE BACK TO FRONTEND
    JsonDocument doc; // Create JSon object
    char message[800]  = {0};

    // Add key:value pairs to Json object according to below schema
    // ‘{"id": "student_id", "timestamp": 1702212234, "number": 9, "ledA": 0, "ledB": 0}’
    doc["id"]         = "620164419"; // Change to your student ID number
    doc["timestamp"]  = getTimeStamp();
    doc["number"]     = number; 
    doc["ledA"]       = getLEDStatus(LED_A); 
    doc["ledB"]       = getLEDStatus(LED_B);
    /*Add code here to insert all other variabes that are missing from Json object
    according to schema above
    */

    serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array  
    publish("topic", message);    // Publish to a topic that only the Frontend subscribes to.
          
  } 

}

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}

//***** Complete the util functions below ******

void Display(unsigned char number){
  /* This function takes an integer between 0 and 9 as input. This integer must be written to the 7-Segment display */
        digitalWrite(a, LOW);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, LOW);
        digitalWrite(e, LOW);
        digitalWrite(f, LOW);
        digitalWrite(g, LOW);
  switch(number){
       
  case 0: 
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(f, HIGH);
        break;
  case 1:
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        break; 
  case 2:
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 3: 
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 4: 
        digitalWrite(b, HIGH);;
        digitalWrite(c, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 5: 
        digitalWrite(a, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 6: 
        digitalWrite(a, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 7: 
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        break;
  case 8: 
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
        break;
  case 9: 
        digitalWrite(a, HIGH);
        digitalWrite(b, HIGH);
        digitalWrite(c, HIGH);
        digitalWrite(d, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
        break;
  default: 
        break; 
  }
 

}

int8_t getLEDStatus(int8_t LED) {
  // RETURNS THE STATE OF A SPECIFIC LED. 0 = LOW, 1 = HIGH
  return digitalRead(LED);
}

void setLEDState(int8_t LED, int8_t state){
  // SETS THE STATE OF A SPECIFIC LED 
  digitalWrite(LED, state);
}

void toggleLED(int8_t LED){
  // TOGGLES THE STATE OF SPECIFIC LED
  int state = !digitalRead(LED);
  digitalWrite(LED, state);
}

void GDP(void){
  // GENERATE, DISPLAY THEN PUBLISH INTEGER

  // GENERATE a random integer 
  number = random(0, 10);
  Serial.println(number);
  Display(number);
  /* Add code here to generate a random integer and then assign 
     this integer to number variable below
  */

  // DISPLAY integer on 7Seg. by 
  
  /* Add code here to calling appropriate function that will display integer to 7-Seg*/

  // PUBLISH number to topic.
  JsonDocument doc; // Create JSon object
  char message[1100]  = {0};

  // Add key:value pairs to Json object according to below schema
  // ‘{"id": "student_id", "timestamp": 1702212234, "number": 9, "ledA": 0, "ledB": 0}’
  doc["id"]         = "620164419"; // Change to your student ID number
  doc["timestamp"]  = getTimeStamp();
  doc["number"]     = number; 
  doc["ledA"]       = getLEDStatus(LED_A); 
  doc["ledB"]       = getLEDStatus(LED_B);
  /*Add code here to insert all other variabes that are missing from Json object
  according to schema above
  */

  serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array
  publish(pubtopic, message);

}
