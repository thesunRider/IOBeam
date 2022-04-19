#include <ESP8266WiFi.h>


const int RSSI_MAX = -50; // define maximum strength of signal in dBm
const int RSSI_MIN = -100; // define minimum strength of signal in dBm

String inData;
String password_wifi;
int index_wifi;
bool wifi_connected;

struct request {
  int command_in;
  String val;
} ;


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println(">begin network driver");
}

/** 0x10 - login scan
    0x20 - scan wifi
           resp 0x53: val: -100 no wifi
           resp 0x53: val: -1 ,data string
           Success , val 1
    0x21 - wifi connect using ssid index and password
          resp  0x53: val: -2 error in request
          Success , val 1

**/
void loop() {

  if (Serial.available() > 0) {
    inData = Serial.readStringUntil('\n');
    if (checkdata(inData)) {
      struct request req = splitdata(inData);
      Serial.print(">command in:");
      Serial.println(req.command_in);
      switch (req.command_in) {
        case 0x10:
          
          break;

        case 0x21:
          WiFi.disconnect();
          index_wifi = req.val.substring(0, req.val.indexOf(";")).toInt();
          if (WiFi.encryptionType(index_wifi) == ENC_TYPE_NONE)
          {
            WiFi.begin(WiFi.SSID(index_wifi), "");
          }
          else
          {
            password_wifi = req.val.substring(req.val.indexOf(";") + 1);
            Serial.print(">WIFI SSID:");
            Serial.println(WiFi.SSID(index_wifi));
            Serial.print(">password:");
            Serial.println(password_wifi);
            WiFi.begin(WiFi.SSID(index_wifi), password_wifi);
          }
           wifi_connected = false;
          for (int i = 0; i < 100 ; i++) {
            if (WiFi.status() == WL_CONNECTED) {
              wifi_connected = true;
              break;
            }
            delay(1000);
          }

          if (wifi_connected) {
            Serial.println("ntwrk:21;1");
          } else {
            Serial.println("> Wifi no internert");
            Serial.println("ntwrk:53;-2");
          }
          break;

        case 0x20:
          Serial.println(">Scanning");
          WiFi.disconnect();

          delay(100);

          WiFi.scanDelete();
          int no_wifi = WiFi.scanNetworks();
          delay(5000);
          String wifi_ssid = "";
          int trim_lngth;


          if (no_wifi <= 0)
          {
            //no device found
            Serial.println("ntwrk:53;-100");
            break;
          }

          Serial.print(">Devices found:");
          Serial.println(no_wifi);

          delay(100);

          for (int i = 0; i < no_wifi; ++i)
          {
            if (WiFi.SSID(i).length() > 9)
              trim_lngth = 9;
            else
              trim_lngth = WiFi.SSID(i).length();

            wifi_ssid += String(i) + "-" + (WiFi.encryptionType(i) == ENC_TYPE_NONE) + "," + WiFi.SSID(i).substring(0, trim_lngth) + " (" + String(dBmtoPercentage(WiFi.RSSI(i))) + "%);";
            delay(100);
          }
          Serial.println("ntwrk:20;1:" + wifi_ssid.substring(0, wifi_ssid.length() - 1));


          break;
      }
    }
  }


}

int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }

  return quality;
}//dBmtoPercentage

bool checkdata(String indata) {
  if (indata.indexOf(":") != -1) {
    if ((indata.substring(0, indata.indexOf(":"))).equals("ntwrk"))
      return true;
  }
  return false;
}


//command structure :    ntwrk:<command_in>;<val>
struct request splitdata(String indata) {
  int command_in = indata.substring(indata.indexOf(":") + 1, indata.indexOf(";")).toInt();
  String val = indata.substring(indata.indexOf(";") + 1);
  struct request req = {command_in, val};

  return req;
}
