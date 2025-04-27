# FrSky S-Port Telemetry Decoder Example

> **Progetto:** Telemetria FrSky SmartPort (S.Port) - Decodifica base per modulo XJT o Multimodulo (es. iRangeX IRX4 Plus)  
> **Autori:**  
> - Codice base (c) Pawelsky 2020  
> - Adattamenti e miglioramenti (c) Giampiero & Blade Runner (ChatGPT) 2025

---

## Descrizione

Questo progetto permette di ricevere e visualizzare dati di telemetria tramite protocollo **FrSky SmartPort (S.Port)**, da un modulo XJT o Multimodulo compatibile, su display **LCD 16x2 I2C** o **OLED SH1106 128x64**.

Supporta i principali dati base (RSSI, RxBatt, SWR, etc.) e funziona su schede **Arduino UNO, Nano, Pro Mini (ATmega328P)** e compatibili.

---

## Librerie necessarie

### Librerie custom incluse:
- `FrSkySportDecoder.h`
- `FrSkySportSensor.h`
- `FrSkySportSensorXjt.h`
- `FrSkySportSingleWireSerial.h`
- `FrSkySportPolling.h`
- `FrSkySportPollingDynamic.h`
- `FrSkySportPollingSimple.h`
- **Nota:** Questi file `.h` e relativi `.cpp` sono inclusi nella cartella principale del pacchetto.

### Librerie standard Arduino:
- `Wire.h` → (Gestione I2C)
- `LiquidCrystal_I2C.h` → (Per display LCD 16x2 I2C)
- `U8g2lib.h` → (Per display OLED SH1106 via I2C)
- `SoftwareSerial.h` → (Solo per Arduino basati su ATmega328P)

---

## Struttura della libreria

```
Frsky_Telemetry/
├── README.md
├── library.properties
├── changelog
├── src/
│   ├── FrSkySportDecoder.h
│   ├── FrSkySportSensor.h
│   ├── FrSkySportSensorXjt.h
│   ├── FrSkySportSingleWireSerial.h
│   ├── FrSkySportPolling.h
│   ├── FrSkySportPollingDynamic.h
│   └── FrSkySportPollingSimple.h
├── examples/
    └── FrSky_Telemetry_Display/
        └── FrSky_Telemetry_Display.ino
```

---

## Come usarla

1. Installa la libreria manualmente:
   - **Arduino IDE → Sketch → #include Library → Add .ZIP Library** → seleziona il file `.zip`.
2. Apri l'esempio:
   - **File → Examples → Frsky_Telemetry → FrSky_Telemetry_Display**
3. Compila e carica sulla tua scheda Arduino.

## Installazione Rapida

Per chi desidera installare rapidamente la libreria nell'IDE Arduino:

Scaricare il file ZIP già pronto:
👉 Frsky_Telemetry_ArduinoLibrary.zip

In Arduino IDE:
Sketch → #include Library → Add .ZIP Library…
e selezionare il file .zip scaricato.


## Note finali

- Se utilizzi Arduino UNO/Nano, ricordati che la memoria RAM è limitata: consigliato l'uso LCD o OLED in modalità `page buffer` (come già configurato).
- Se usi il Multi-Modulo, verifica che sia configurato correttamente per inviare i dati via S.Port.

