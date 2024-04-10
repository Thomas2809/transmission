

#include "transmission.h"

//************* DEFINITION DES OBJETS ************
BluetoothSerial SerialBT;
M5EPD_Canvas canvas(&M5.EPD);

//************* DEFINITION DES constantes ************

const char *pin = "1234";  // Change this to reflect the pin expected by the real slave BT device
String myName = "ESP32-BT-Master";
String slaveName = "ESP32-BT-Slave";  // Change this to reflect the real name of your slave BT device

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

void init_screen(int sens_Sc,int sens_Tp, int largeur, int hauteur, int texte_size)
{

    M5.EPD.SetRotation(sens_Sc);
    M5.TP.SetRotation(sens_Tp);
    M5.EPD.Clear(true);
    canvas.createCanvas(largeur, hauteur);
    canvas.setTextSize(texte_size);
}
