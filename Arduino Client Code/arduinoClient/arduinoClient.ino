/** @file arduinoClient.ino
 *  @brief Arduino Client code
 *
 *  This code runs at client which has GPS attached and sends
 *  location information to the server by HTTP request using
 *  GSM(2G) connection 

 *  Note:
 *    1. GPS module and GSM module required with a Arduino.
 *    2. Use SIM with 2G capability and refer AT commands manual
 *       if your getting AT errors 
 *        
 *  @author Amrathesh
 */
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

/*Arduino PIN assignments*/
#define SS_TX_GPS 4
#define SS_RX_GPS 3

#define SS_TX_GSM 10
#define SS_RX_GSM 9

#define REFRESH_DELAY 20000

#define DEVICE_ID 9

#define GPS_BAUD 9600
#define GSM_BAUD 9600
#define PC_BAUD  9600

#define VALUE_PRECISION 8
#define DEBUG 1

float gpsLatitude, gpsLongitude;
char *atCommands[]= {"AT",
                     "AT+CPIN?",
                     "AT+CREG?",
                     "AT+CGATT?",
                     "AT+CSQ",
                     "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
                     "AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"",
                     "AT+SAPBR=1,1",
                     "AT+SAPBR=2,1",
                     "AT+HTTPINIT",
                     "AT+HTTPPARA=\"CID\",1",
                     "AT+HTTPPARA=\"URL\",\"childmonitorsystem.000webhostapp.com/p.php\"",  //Server address
                     "AT+HTTPPARA=\"CONTENT\",\"application/json\"",
                     "dummy",
                     "AT+HTTPACTION=1",
                     "AT+HTTPREAD",
                     "AT+HTTPTERM",
                     };


TinyGPSPlus gps;
/* TX RX w.r.t Arduino */
SoftwareSerial gpsSerial(SS_RX_GPS, SS_TX_GPS);
SoftwareSerial gsmSerial(SS_RX_GSM, SS_TX_GSM);

StaticJsonBuffer<200> jsonBuffer;

void setup()
{
  Serial.begin(PC_BAUD);
  DynamicJsonBuffer jsonBuffer;
  
}

void sampleGPS()
{
  gsmSerial.end();
  gpsSerial.begin(GPS_BAUD);
  while (gpsSerial.available() > 0)
      gps.encode(gpsSerial.read());
  if (gps.location.lat() == 0) {
      if(DEBUG) Serial.println("GPS not Fixed yet, check connections");
      return sampleGPS();
  }
  gpsLatitude  = gps.location.lat();
  gpsLongitude = gps.location.lng();
  if(DEBUG) {
      Serial.print("Latitude:");
      Serial.println(gpsLatitude,VALUE_PRECISION);
      Serial.print("Longitude:");
      Serial.println(gpsLongitude,VALUE_PRECISION);
  }
  gpsSerial.end();
  gsmSerial.begin(GSM_BAUD);
  return;
}

void readGSM()
{
 while (gsmSerial.available()>0)
      Serial.write(gsmSerial.read());
 delay(1000);
}

void sendAtCmd(String cmd)
{
  gsmSerial.println(cmd);
  delay(6000);
}

void loop ()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  
  sampleGPS();
  delay(10000);


  object.set("lat",gpsLatitude);
  object.set("lon",gpsLongitude);
  object.set("devid",DEVICE_ID);
  
  object.printTo(Serial);
  Serial.println(" ");  
  String sendtoserver;
  object.prettyPrintTo(sendtoserver);
  delay(4000);

  for (int i = 0; i < 17; i++) {
      if(i == 13) {
         sendAtCmd("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
         readGSM();
         sendAtCmd(sendtoserver);
         readGSM();
         continue;
      }
      sendAtCmd(atCommands[i]);
      delay(1000);
      readGSM();    
  }
  
  delay(REFRESH_DELAY);
}
