/*
  FrSky S-Port Telemetry Decoder example for XJT Module or Multi module tipo IRangeX IRX4 PLUS o Simili
  (c) Pawelsky 2020, adattamenti (c) Giampiero & Blade Runner (ChatGPT) 2025

  - Librerie custom necessarie:
    * FrSkySportDecoder.h
    * FrSkySportSensor.h
    * FrSkySportSensorXjt.h
    * FrSkySportSingleWireSerial.h

  - Librerie standard:
    * Wire.h
    * LiquidCrystal_I2C.h (per LCD)
    * U8g2lib.h (per OLED)
    * SoftwareSerial.h (solo per ATmega328P)

  Richiede Arduino Uno/Nano/Pro Mini (ATmega328P) o schede compatibili. 
  Usa il protocollo FrSky SmartPort (S.Port) per ricevere dati di telemetria.
*/

// Uncomment the #define below to enable internal polling of data.
// Use only when there is no device in the S.Port chain (e.g. S.Port capable FrSky receiver) that normally polls the data.
//#define POLLING_ENABLED

unsigned long lastPacketTime = 0;  // Memorizza il tempo dell'ultimo pacchetto ricevuto

// Scegli il tipo di display da usare
#define USE_LCD      // LCD 16x2 via I2C
//#define USE_OLED   // OLED SSD1306 128x64 via I2C

#ifdef USE_LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define LED_PIN 3
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

#ifdef USE_OLED
#include <Wire.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

#include "FrSkySportSensor.h"
#include "FrSkySportSensorXjt.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportDecoder.h"

#if defined(__AVR_ATmega328P__)
#include <SoftwareSerial.h>
#endif

FrSkySportSensorXjt xjt;
#ifdef POLLING_ENABLED
  #include "FrSkySportPollingDynamic.h"
  FrSkySportDecoder decoder(new FrSkySportPollingDynamic());
#else
  FrSkySportDecoder decoder;
#endif

uint32_t currentTime, displayTime;
uint16_t decodeResult;
float xxx = 0;

void setup()
{
  #ifdef USE_LCD
  lcd.init();
  lcd.backlight();
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 100);
  #endif

  #ifdef USE_OLED
  display.begin();
  display.clearDisplay();
  display.sendBuffer();
  #endif

  #if defined(__AVR_ATmega328P__)
  decoder.begin(FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_12, &xjt);
  #else
  decoder.begin(FrSkySportSingleWireSerial::SERIAL_3, &xjt);
  #endif

  Serial.begin(115200);
}

void loop()
{
  decodeResult = decoder.decode(); 

  if (decodeResult != SENSOR_NO_DATA_ID) 
  {
    lastPacketTime = millis();
    Serial.print("Decoded data with AppID 0x");
    Serial.println(decodeResult, HEX);
  }

  currentTime = millis();

  if (currentTime > displayTime)
  {
    displayTime = currentTime + 1000;

    Serial.println("");

    Serial.print("Basic: RSSI = "); Serial.print(xjt.getRssi());
    Serial.print(", ADC1 = "); Serial.print(xjt.getAdc1());
    Serial.print("V, ADC2 = "); Serial.print(xjt.getAdc2());
    Serial.print("V, RxBatt = "); Serial.print(xjt.getRxBatt());
    Serial.print("V, SWR = "); Serial.println(xjt.getSwr());

    Serial.print("FGS: fuel = "); Serial.print(xjt.getFuel()); Serial.println("%");
    Serial.print("FLVS: cell1 = "); Serial.print(xjt.getCell1()); Serial.print("V, cell2 = "); Serial.print(xjt.getCell2());
    Serial.print("V, cell12 = "); Serial.print(xjt.getCell12()); Serial.println("V");

    Serial.print("FAS: current = "); Serial.print(xjt.getCurrent());
    Serial.print("A, voltage = "); Serial.print(xjt.getVoltage()); Serial.println("V");

    Serial.print("FVAS: altitude = "); Serial.print(xjt.getAltitude()); Serial.print("m");
    Serial.print(", VSI = "); Serial.print(xjt.getVsi()); Serial.println("m/s");

    Serial.print("GPS: lat = "); Serial.print(xjt.getLat(), 6);
    Serial.print(", lon = "); Serial.print(xjt.getLon(), 6);
    Serial.print(", altitude = "); Serial.print(xjt.getAltitude());
    Serial.print("m, speed = "); Serial.print(xjt.getSpeed());
    Serial.print("m/s, COG = "); Serial.print(xjt.getCog());
    char dateTimeStr[18]; 
    sprintf(dateTimeStr, "%02u-%02u-%04u %02u:%02u:%02u", xjt.getDay(), xjt.getMonth(), xjt.getYear() + 2000, xjt.getHour(), xjt.getMinute(), xjt.getSecond());
    Serial.print(", date/time = "); Serial.println(dateTimeStr);

    Serial.print("TAS: ACCX = "); Serial.print(xjt.getAccX());
    Serial.print("G, ACCY = "); Serial.print(xjt.getAccY());
    Serial.print("G, ACCZ = "); Serial.println(xjt.getAccZ());

    Serial.print("TEMS: T1 = "); Serial.print(xjt.getT1());
    Serial.print(" deg. C, T2 = "); Serial.print(xjt.getT2()); Serial.println(" deg. C");

    Serial.print("RPMS: RPM = "); Serial.println(xjt.getRpm());
    Serial.println("");

    if ((currentTime - lastPacketTime > 6000))
    {
      #ifdef USE_LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NO SIGNAL");
      lcd.setCursor(0, 1);
      lcd.print("...");
      #endif

      #ifdef USE_OLED
      display.firstPage();
      do {
        display.setFont(u8g2_font_ncenB14_tr);
        display.drawStr(0, 16, "NO SIGNAL");
        display.setFont(u8g2_font_ncenB08_tr);
        display.drawStr(0, 40, "...");
      } while (display.nextPage());
      #endif
    }
    else
    {
      #ifdef USE_LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RSSI ");
      lcd.print(xjt.getRssi());
      lcd.setCursor(9, 0);
      lcd.print("SWR ");
      lcd.print(xjt.getSwr());

      lcd.setCursor(0, 1);
      lcd.print("RxBatt:");
      xxx = (xjt.getRxBatt() * 1.05);
      lcd.print(xxx, 2);
      lcd.setCursor(14, 1);
      lcd.print("V");
      #endif

      #ifdef USE_OLED
      display.firstPage();
      do {
        display.setFont(u8g2_font_6x10_tr);

        char buffer[20];
        sprintf(buffer, "RSSI:%d  SWR:%d", xjt.getRssi(), xjt.getSwr());
        display.drawStr(0, 10, buffer);

        xxx = (xjt.getRxBatt() * 1.05);
        char battStr[10];
        dtostrf(xxx, 5, 2, battStr);
        sprintf(buffer, "RxBatt:%sV", battStr);
        display.drawStr(0, 26, buffer);

      } while (display.nextPage());
      #endif
    }
  }
}

