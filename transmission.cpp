

#include "transmission.h"

//************* DEFINITION DES OBJETS ************
BluetoothSerial SerialBT;
M5EPD_Canvas canvas(&M5.EPD);
DFRobot_ADS1115 ads1(&secondWire);
DFRobot_ADS1115 ads2(&secondWire);

//************* DEFINITION DES constantes ************

const char *pin = "1234";  // Change this to reflect the pin expected by the real slave BT device
String myName = "ESP32-BT-Master";
String slaveName = "ESP32-BT-Slave";  // Change this to reflect the real name of your slave BT device

//************* Initialisation bluetooth ************

void init_bluetooth() {

  static bool connected;

  SerialBT.begin(myName, true);

  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());
  SerialBT.setPin(pin);
  Serial.println("Using PIN");

  // connect(address) is fast (up to 10 secs max), connect(slaveName) is slow (up to 30 secs max) as it needs
  // to resolve slaveName to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices Bluetooth address and device names
  connected = SerialBT.connect(slaveName);
  Serial.printf("Connecting to slave BT device named \"%s\"\n", slaveName.c_str());
  if (connected) {
    Serial.println("Connected Successfully!");
  } else {

    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }

  // Disconnect() may take up to 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Successfully!");
  }
  // This would reconnect to the slaveName(will use address, if resolved) or address used with connect(slaveName/address).
  SerialBT.connect();

  if (connected) {
    Serial.println("Reconnected Successfully!");
  } else {

    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to reconnect. Make sure remote device is available and in range, then restart app.");
    }
  }
}

//************* Initialisation de l'ecran ************

void init_screen(int sens_Sc, int sens_Tp, int largeur, int hauteur, int texte_size) {

  M5.EPD.SetRotation(sens_Sc);
  M5.TP.SetRotation(sens_Tp);
  M5.EPD.Clear(true);
  canvas.createCanvas(largeur, hauteur);
  canvas.setTextSize(texte_size);
}

void init_ads1() {

  ads1.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);  // 0x49
  ads1.setGain(eGAIN_TWOTHIRDS);   // Définit le gain à deux tiers
  ads1.setMode(eMODE_SINGLE);      // Définition du mode de fonctionnement en mode unique
  ads1.setRate(eRATE_128);         // Taux d'échantillonnage à 128SPS (défaut)
  ads1.setOSMode(eOSMODE_SINGLE);  // Définition du mode de déclenchement en mode unique
}

void init_ads2() {
  ads2.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
  ads2.setGain(eGAIN_TWOTHIRDS);
  ads2.setMode(eMODE_SINGLE);
  ads2.setRate(eRATE_128);
  ads2.setOSMode(eOSMODE_SINGLE);
}