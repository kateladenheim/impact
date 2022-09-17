#include "WiFi.h" // ESP32 WiFi include

const char* ssid = "7117-2G Guest";
const char* password = "djf112430";

#define fsrPin1 A0     // this FSR [velostat strip] is connected to A0
#define fsrPin2 A1     // this FSR [velostat strip] is connected to A1
#define fsrPin3 A2     // this FSR [velostat strip] is connected to A2
#define fsrPin4 A3     // this FSR [velostat strip] is connected to A3
#define fsrPin5 A4     // this FSR [velostat strip] is connected to A4
#define fsrPin6 13     // this FSR [velostat strip] is connected to 13
#define fsrPin7 12     // this FSR [velostat strip] is connected to 12
#define fsrPin8 27     // this FSR [velostat strip] is connected to 27
#define fsrPin9 33     // this FSR [velostat strip] is connected to 33
#define fsrPin10 15     // this FSR [velostat strip] is connected to 15

//int in1 = 7; // relay pin


int fsrReading1;     // the analog reading from the FSR resistor divider
int fsrReading2;     // the analog reading from the FSR resistor divider
int fsrReading3;     // the analog reading from the FSR resistor divider
int fsrReading4;     // the analog reading from the FSR resistor divider
int fsrReading5;     // the analog reading from the FSR resistor divider
int fsrReading6;     // the analog reading from the FSR resistor divider
int fsrReading7;     // the analog reading from the FSR resistor divider
int fsrReading8;     // the analog reading from the FSR resistor divider
int fsrReading9;     // the analog reading from the FSR resistor divider
int fsrReading10;     // the analog reading from the FSR resistor divider

void ConnectToWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to "); Serial.println(ssid);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);

    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }

  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
//    while(!Serial);
//    Serial.println("Why doesn't this stupid thing work");

    ConnectToWiFi();

}
void loop() {
  // put your main code here, to run repeatedly:

  // get reading numbers 
  
  fsrReading1 = analogRead(fsrPin1);  
 
  Serial.print("Analog reading fsr1 = ");
  Serial.println(fsrReading1);     // print the raw analog reading

    fsrReading2 = analogRead(fsrPin2);  
 
  Serial.print("Analog reading fsr2 = ");
  Serial.println(fsrReading2);     // print the raw analog reading

      fsrReading3 = analogRead(fsrPin3);  
 
  Serial.print("Analog reading fsr3 = ");
  Serial.println(fsrReading3);     // print the raw analog reading

      fsrReading4 = analogRead(fsrPin4);  
 
  Serial.print("Analog reading fsr4 = ");
  Serial.println(fsrReading4);     // print the raw analog reading

      fsrReading5 = analogRead(fsrPin5);  
 
  Serial.print("Analog reading fsr5 = ");
  Serial.println(fsrReading5);     // print the raw analog reading

        fsrReading6 = analogRead(fsrPin6);  
 
  Serial.print("Analog reading fsr6 = ");
  Serial.println(fsrReading6);     // print the raw analog reading

        fsrReading7 = analogRead(fsrPin7);  
 
  Serial.print("Analog reading fsr7 = ");
  Serial.println(fsrReading7);     // print the raw analog reading

          fsrReading8 = analogRead(fsrPin8);  
 
  Serial.print("Analog reading fsr8 = ");
  Serial.println(fsrReading8);     // print the raw analog reading

          fsrReading9 = analogRead(fsrPin9);  
 
  Serial.print("Analog reading fsr9 = ");
  Serial.println(fsrReading9);     // print the raw analog reading

          fsrReading10 = analogRead(fsrPin10);  
 
  Serial.print("Analog reading fsr10 = ");
  Serial.println(fsrReading10);     // print the raw analog reading


  // conditional logic: trigger if 012, 123, or 234 meet pressure threshold, otherwise do not trigger
  
  
if ( (fsrReading1 > 3900 && fsrReading2 >= 3600 && fsrReading3 > 3900 && fsrReading6 > 3900 && fsrReading7 > 3900 && fsrReading8 > 3900) ) {
    Serial.println(" - go");
//    CODE TO TRIGGER SERVER SIGNAL
  }

else if ( (fsrReading2 >= 3600 && fsrReading3 > 3900 && fsrReading4 >= 3900 && fsrReading7 > 3900 && fsrReading8 > 3900 && fsrReading9 > 3900) ) {
    Serial.println(" - go");
//    CODE TO TRIGGER SERVER SIGNAL
}

else if ( (fsrReading3 > 3600 && fsrReading4 >= 3900 && fsrReading5 >= 3900 && fsrReading8 > 3900 && fsrReading9 > 3900 && fsrReading10 > 3900) ) {
    Serial.println(" - go");
//    CODE TO TRIGGER SERVER SIGNAL
}

else {
    Serial.println(" - stop");
//    digitalWrite(in1, LOW);

  }

 
  delay(1000);

}
