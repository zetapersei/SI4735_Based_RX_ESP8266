/*
   Test and validation of the SI4735 Arduino Library and ESP8266.

   ATTENTION:  Please, avoid using the computer connected to the mains during testing.

   The main advantages of using this sketch are: 
    1) It is an easy way to check if your circuit is working
    2) You do not need to connect any display device to make your radio works
    3) You do not need connect any push buttons or encoders to change volume and frequency
    4) The Arduino IDE is all you need to control the radio
   
   This sketch has been successfully tested on ESP8266 on SI4735-G60 or SI4732-A10

  | Si4735-G60 | Function              |   ESP8266             |
  |------------| ----------------------|-----------------------|
  | pin 15     |   RESET               |   2 (GPIO2)           |  
  | pin 18     |   SDIO                |   4 (SDA / GPIO4)     |
  | pin 17     |   SCLK                |   5 (SCL / GPIO5)     |


  | Si4732-A10 | Function              |   ESP8266             |
  |------------| ----------------------|-----------------------|
  | pin 9      |   RESET               |   2 (GPIO2)           |  
  | pin 12     |   SDIO                |   4 (SDA / GPIO4)     |
  | pin 11     |   SCLK                |   5 (SCL / GPIO5)     |



  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, May 2021.
*/

#include <SI4735.h>

#define RESET_PIN 2          // GPIO2
#define ESP32_I2C_SDA 4       // GPIO4
#define ESP32_I2C_SCL 5       // GPIO5
#define AM_FUNCTION 1
#define FM_FUNCTION 0
#define BANDWIDTH_COUNT 7

uint16_t currentFrequency = 0;
uint16_t previousFrequency = 0;
uint8_t bandwidthIdx = 0;
const char *const bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

SI4735 si4735;

void showHelp()
{
  Serial.println(F("Type F to FM; A to MW; L to LW; and 1 to SW"));
  Serial.println(F("Type U to increase and D to decrease the frequency"));
  Serial.println(F("Type S or s to seek station Up or Down"));
  Serial.println(F("Type + or - to volume Up or Down"));
  Serial.println(F("Type 0 to show current status"));
  Serial.println(F("Type B to change Bandwidth filter"));
  Serial.println(F("Type ? to this help."));
  Serial.println(F("=================================================="));
  delay(1000);
}

void showStatus()
{
  si4735.getStatus();
  si4735.getCurrentReceivedSignalQuality();

  char buffer[80];
  if (si4735.isCurrentTuneFM())
  {
    snprintf(buffer, sizeof(buffer),
             "You are tuned on %.2fMHz %s [SNR:%ddB Signal:%ddBuV]",
             currentFrequency / 100.0f,
             (si4735.getCurrentPilot()) ? "STEREO" : "MONO",
             si4735.getCurrentSNR(),
             si4735.getCurrentRSSI());
  }
  else
  {
    snprintf(buffer, sizeof(buffer),
             "You are tuned on %u kHz [SNR:%ddB Signal:%ddBuV]",
             currentFrequency,
             si4735.getCurrentSNR(),
             si4735.getCurrentRSSI());
  }

  Serial.println(buffer);
  Serial.flush();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(1000);

  digitalWrite(RESET_PIN, HIGH);
  Serial.println(F("AM and FM station tuning test."));
  showHelp();

  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);
  si4735.getDeviceI2CAddress(RESET_PIN);
  si4735.setup(RESET_PIN, FM_FUNCTION);
  delay(300);

  si4735.setFM(8400, 10800, 10390, 10);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}

void loop()
{
  if (Serial.available() > 0)
  {
    const char key = Serial.read();
    switch (key)
    {
      case '+':
        si4735.volumeUp();
        break;
      case '-':
        si4735.volumeDown();
        break;
      case 'a':
      case 'A':
        si4735.setAM(570, 1710, 810, 10);
        break;
      case 'f':
      case 'F':
        si4735.setFM(8600, 10800, 10390, 10);
        break;
      case '1':
        si4735.setAM(9400, 9990, 9600, 5);
        break;
      case 'u':
      case 'U':
        si4735.frequencyUp();
        break;
      case 'd':
      case 'D':
        si4735.frequencyDown();
        break;
      case 'b':
      case 'B':
        if (si4735.isCurrentTuneFM())
        {
          Serial.println(F("Not valid for FM"));
        }
        else
        {
          if (bandwidthIdx >= BANDWIDTH_COUNT)
            bandwidthIdx = 0;

          si4735.setBandwidth(bandwidthIdx, 1);
          Serial.print(F("Filter - Bandwidth: "));
          Serial.print(bandwidth[bandwidthIdx]);
          Serial.println(F(" kHz"));
          bandwidthIdx++;
        }
        break;
      case 'S':
        si4735.seekStationUp();
        break;
      case 's':
        si4735.seekStationDown();
        break;
      case '0':
        showStatus();
        break;
      case '?':
        showHelp();
        break;
      default:
        break;
    }
  }

  const uint16_t nextFrequency = si4735.getCurrentFrequency();
  if (nextFrequency != previousFrequency)
  {
    previousFrequency = nextFrequency;
    currentFrequency = nextFrequency;
    showStatus();
    delay(300);
  }

  delay(5);
}
