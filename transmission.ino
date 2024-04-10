//***************************************************************************
// envoie_donne_m5paper.ino
// Main_Robotique
// Giarrizzo Thomas
// 6 ème électronique Inraci
// Hardwarde: Arduino_feather_ESP32, M5Stack_Paper, Adc1115(I2C), Servomoteurs, capteur_de_fexion_ZD10-100,
// 18 mars 2024


//*********************************Librairie*********************************

#include "BluetoothSerial.h"
#include <Wire.h>
#include "DFRobot_ADS1115.h"
#include <M5EPD.h>


//C'est 


//********************************Constante**********************************
#define NB_data 7
//#define debug_ADC
const char *pin = "1234";             // Change this to reflect the pin expected by the real slave BT device

int point[2][2];

//********************************Variable***********************************
char sendtab[NB_data];
String slaveName = "ESP32-BT-Slave";  // Change this to reflect the real name of your slave BT device
String myName = "ESP32-BT-Master";
int16_t adc0, adc1, adc2;
int16_t adc6, adc7; //   _t:

//*******************************Objet****************************************
BluetoothSerial SerialBT;
M5EPD_Canvas canvas(&M5.EPD);

DFRobot_ADS1115 ads1(&secondWire);
DFRobot_ADS1115 ads2(&secondWire);


esp_timer_handle_t timer;
//******************************
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
//***************************************Interuption***************************
// cette fonction est appelé à chaque déclanchement du timer
void IRAM_ATTR onTimer(void *param) {


  static unsigned int cpt = 0;
  //C'est ici que tu écris ce que tu veux qui arrive toutes les 50ms
  Serial.printf("%d-> ",cpt++);// incrémante le compteurs


   if (ads2.checkADS1115())
  {   // gère les données du deuxième module


    adc7 = ads2.readVoltage(3);
    Serial.printf("A03:%5d ",adc7 / 25);

    adc6 = ads2.readVoltage(2);
    Serial.printf("A02:%5d ",adc6 / 25);
  }
  else {
    Serial.println("ADS1115-gauche Disconnected!");
  }

  if (ads1.checkADS1115())
  {   // gère les données du premier module

    adc2 = ads1.readVoltage(2);
    Serial.printf("A12:%5d ",adc2 / 25);

    adc1 = ads1.readVoltage(1);
    Serial.printf("A11:%5d ",adc1 / 25);

    adc0 = ads1.readVoltage(0);
    Serial.printf("A10:%5d \n",255- adc0/25);
  }
  else
  {
    Serial.println("ADS1115-droite Disconnected!");
  }

  sendtab[0] = '#';
  sendtab[1] = char(255 - adc0 / 25);  //les données doivent être placé ici ...
  sendtab[2] = char(adc1 / 25); // /25 car pas besoin de 6123 positions.
  sendtab[3] = char(adc2 / 25);
  sendtab[4] = char(adc6 / 25);
  sendtab[5] = char(adc7 / 25);
  sendtab[6] = sendtab[1] ^ sendtab[2] ^ sendtab[3] ^ sendtab[4] ^ sendtab[5];  //byte de controle d'intégrité de donnée

  for (char cptSend = 0; cptSend < NB_data; cptSend++)
  {
    SerialBT.print(sendtab[cptSend]);
    #ifdef debug_ADC
      Serial.println(sendtab[cptSend]);
    #endif
  }

}
//*******************************Initiation**************************************
void setup() {
  bool connected;



  delay(100);

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

  M5.begin();
  secondWire.begin(I2C_SDA, I2C_SCL, (uint32_t)400000U); // Initialisation du deuxième bus I2C avec les broches SDA et SCL définies
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  canvas.createCanvas(540, 960);
  canvas.setTextSize(5);


  ads1.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);  // 0x49
  ads2.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48

  ads1.setGain(eGAIN_TWOTHIRDS); // Définit le gain à deux tiers
  ads1.setMode(eMODE_SINGLE); // Définition du mode de fonctionnement en mode unique
  ads1.setRate(eRATE_128);  // Taux d'échantillonnage à 128SPS (défaut)
  ads1.setOSMode(eOSMODE_SINGLE);  // Définition du mode de déclenchement en mode unique


  ads2.setGain(eGAIN_TWOTHIRDS);
  ads2.setMode(eMODE_SINGLE);
  ads2.setRate(eRATE_128);
  ads2.setOSMode(eOSMODE_SINGLE);


// Configuration de l'interruption du timer

  esp_timer_create_args_t timerArgs; // crée une structure pour configurer le timer
  timerArgs.callback = &onTimer; // Définition de la fonction de rappel
  timerArgs.arg = NULL; // rien de plus n'est passé à la fonction de rappel
  esp_timer_create(&timerArgs, &timer); //Création du timer
  esp_timer_start_periodic(timer, 20000);  // Déclencher toutes les 20 ms

}


void loop() {

static int cpt=0;
 /*
  canvas.drawString("ADS1", 10, 0);
  canvas.drawString("A0: " + String(255 - adc0 / 25) + "mV", 10, 60);
  canvas.drawString("A1: " + String(adc1 / 25) + "mV", 10, 110);
  canvas.drawString("A2: " + String(adc2 / 25) + "mV", 10, 150);
  canvas.drawString("ADS2", 10, 210);
  canvas.drawString("A2: " + String(adc6 / 25) + "mV", 10, 260);
  canvas.drawString("A3: " + String(adc7 / 25) + "mV", 10, 300);

  canvas.drawString("Life CPT: " + String(cpt++), 10, 360);
  canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
  delay(20);  // Ajout d'un délai
  */
  // SerialBT.println('@');
  //Serial.println('@');

  if (M5.TP.available()) {
        if (!M5.TP.isFingerUp()) {
            M5.TP.update();
            canvas.fillCanvas(0);
            bool is_update = false;
            for (int i = 0; i < 2; i++) {
                tp_finger_t FingerItem = M5.TP.readFinger(i);
                if ((point[i][0] != FingerItem.x) ||
                    (point[i][1] != FingerItem.y)) {
                    is_update   = true;
                    point[i][0] = FingerItem.x;
                    point[i][1] = FingerItem.y;
                    canvas.fillRect(FingerItem.x - 50, FingerItem.y - 50, 100,
                                    100, 15);
                    Serial.printf("Finger ID:%d-->X: %d*C  Y: %d  Size: %d\r\n",FingerItem.id, FingerItem.x, FingerItem.y,FingerItem.size);
                    canvas.drawString("X: "+ String(FingerItem.x) + " " "Y:"+String(FingerItem.y), 10, 60);
                }
            }
            if (is_update) {
                canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
            }
        }
    }
}
