#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "index.h" 

// Wi-Fi AP ayarları
const char* ssid = "microbot";
const char* password = "12345678";

const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer webServer(80);
int rawSpeed;
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
bool hornState = 0;
bool isHornStateChanged = 0;
// MOTOR A SOL MOTOR MOTOR B SAG MOTOR
#define ena 5
#define in1 13
#define in2 2
#define enb 4
#define in3 14
#define in4 12
#define horn 0


void setup() {
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  //pinMode(horn, OUTPUT);

  Serial.begin(115200);

  // Access Point başlatma
  WiFi.softAP(ssid, password);
  Serial.println("Access Point oluşturuldu.");
  Serial.print("SSID: ");
  Serial.println(ssid);

  // Tüm domainleri Captive Portal'a yönlendirme
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Web Server ayarları
  webServer.on("/", [](){
    webServer.send(200, "text/html", MAIN_page);    
  });
  webServer.on("/horn", [](){
    if(webServer.arg("i") == "1"){
      Serial.println("basildi");
      hornState = 1;
      isHornStateChanged = 1;
    }
    if(webServer.arg("i") == "0"){
      Serial.println("cekildi");
      isHornStateChanged = 1;
      hornState = 0;
      
    }
    webServer.send(200, "text/json", "{result:1}");  
  });
  webServer.on("/rightmotor", [](){
    rawSpeed = webServer.arg("s").toInt();
    if(rawSpeed>255){ // sag motor ileri gitmek istiyor
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
      rightMotorSpeed = rawSpeed-255;  
    } 
    else if(rawSpeed == 255){ // durdur
      digitalWrite(in3,HIGH);
      digitalWrite(in4,HIGH);
      rightMotorSpeed = 0; 
    }
    else{
      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);
      rightMotorSpeed = 255-rawSpeed; 
    }
    webServer.send(200, "text/json", "{result:1}");    
  });
  webServer.on("/leftmotor", [](){
    rawSpeed = webServer.arg("s").toInt();
    if(rawSpeed>255){ // sag motor ileri gitmek istiyor
      
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      leftMotorSpeed = rawSpeed-255;  
      Serial.println(leftMotorSpeed);
    } 
    else if(rawSpeed == 255){ // durdur
      digitalWrite(in1,HIGH);
      digitalWrite(in2,HIGH);
      leftMotorSpeed = 0; 
    }
    else{
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      leftMotorSpeed = 255-rawSpeed; 
    }
    webServer.send(200, "text/json", "{result:1}");    
  });
  webServer.on("/rgb", [](){
    Serial.println("LED "+webServer.arg("r")+" "+webServer.arg("g")+" "+webServer.arg("b"));
    webServer.send(200, "text/json", "{result:1}");    
  });
  webServer.onNotFound([](){
    webServer.send(200, "text/html", MAIN_page);    
  });
  webServer.begin();
  Serial.println("Web Server çalışıyor...");
}

void loop() {
  dnsServer.processNextRequest(); // DNS isteklerini işleme
  webServer.handleClient();       // Web Server isteklerini işleme
  analogWrite(enb,rightMotorSpeed);
  analogWrite(ena,leftMotorSpeed);
  //Serial.println("A:255,U:0,R:"+String(rightMotorSpeed)+",L:"+String(leftMotorSpeed));
  if(isHornStateChanged){
    if(hornState){
     Serial.println("HORN 1");
   }
   else{
     Serial.println("HORN 0");
   }
   isHornStateChanged=0;
  }
   
}
