#include <DFRobot_SIM808.h>
#include <SoftwareSerial.h>
#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char MESSAGE[300];
char lat[12];
char lon[12];
char wspeed[12];
char phone[16];
char datetime[24];
#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial mySerial(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&mySerial);
char msg;
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  while (!sim808.init())
  {
    Serial.print("Sim808 init error\r\n");
    delay(1000);
  }
  delay(3000);

  if ( sim808.attachGPS())
    Serial.println("Open the GPS power success");
  else
    Serial.println("Open the GPS power failure");

  Serial.println("Init Success, please send SMS message to me!");
}

void loop()
{
  int x=1;
 if(x==1)
 {
    SendgpsMessage();
 } 
  //MakeCall();
  //delay(5000);
}





void SendgpsMessage()
{
  messageIndex = sim808.isSMSunread();
  if (messageIndex > 0)
  {
    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
    sim808.deleteSMS(messageIndex);
    while (!sim808.getGPS())
    {
    }
    float la = sim808.GPSdata.lat;
    float lo = sim808.GPSdata.lon;
    float ws = sim808.GPSdata.speed_kph;
    dtostrf(la, 6, 2, lat);
    dtostrf(lo, 6, 2, lon);
    dtostrf(ws, 6, 2, wspeed);
    sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nTry With This Link.\nhttp://www.latlong.net/Show-Latitude-Longitude.html", lat, lon);
    sim808.sendSMS(phone, MESSAGE);
    //sim808.detachGPS();
  }
}
void MakeCall()
{
  mySerial.println("ATD+ZZZXXXXXXXXX;");//Replace with phone number 
  Serial.println("Calling  ");
  delay(1000);
}
