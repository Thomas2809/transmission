//***************************************************************************
// transmission.ino
// Main_Robotique
// Giarrizzo Thomas
// 6 ème électronique Inraci
// Hardwarde: Arduino_feather_ESP32, M5Stack_Paper, Adc1115(I2C), Servomoteurs, capteur_de_fexion_ZD10-100,
// 18 mars 2024


//*********************************Librairie*********************************

#include "BluetoothSerial.h"
#include <Wire.h>
#include "transmission.h"

//********************************Constante**********************************
#define NB_data 7
//#define debug_ADC
#define PIN_G33 33

#define MODE_MANUEL 1
#define MODE_BT 2
#define MODE_MENU 0

//********************************Variable***********************************
char sendtab[NB_data];

int16_t adc0, adc1, adc2;
int16_t adc6, adc7;  
int point2[2][2];
byte Mode = MODE_MENU;
bool inOptionsMenu = false;
//*******************************Objet****************************************
//BluetoothSerial SerialBT;
esp_timer_handle_t timer;
//******************************
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

//***************************************Interuption***************************
// cette fonction est appelé à chaque déclanchement du timer
void IRAM_ATTR onTimer(void *param) {
  //*******************modid porte ouverte****************************
  static char ADC1[10] = { 245, 245, 245, 245, 245, 245, 245, 245, 245, 245 };
  static char indice = 9;  // dernier indice

  indice++;
  if (indice >= 10)
    indice = 0;
  adc1 = ads1.readVoltage(1);
  Serial.printf("A11:%5d ", adc1 / 25);

  ADC1[indice] = char(adc1 / 25);

  int somme = 0;


  for (int i = 0; i < 10; i++) {
    somme += ADC1[i];
  }
  int moyenne = somme / 10;
  Serial.print(moyenne);

  static unsigned int cpt = 0;
  //C'est ici que tu écris ce que tu veux qui arrive toutes les 50ms
  Serial.printf("%d-> ", cpt++);  // incrémante le compteurs

  if (ads2.checkADS1115()) {  // gère les données du deuxième module


    adc7 = ads2.readVoltage(3);
    Serial.printf("A03:%5d ", adc7 / 25);

    adc6 = ads2.readVoltage(2);
    Serial.printf("A02:%5d ", adc6 / 25);
  } else {
    Serial.println("ADS1115-gauche Disconnected!");
  }

  if (ads1.checkADS1115()) {  // gère les données du premier module

    adc2 = ads1.readVoltage(2);
    Serial.printf("A12:%5d ", adc2 / 25);
    //digitalWrite(PIN_G33, HIGH); //1
    adc1 = ads1.readVoltage(1);
    Serial.printf("A11:%5d ", adc1 / 25);
    //digitalWrite(PIN_G33, LOW); //0
    adc0 = ads1.readVoltage(0);
    Serial.printf("A10:%5d \n", 255 - adc0 / 25);
  } else {
    Serial.println("ADS1115-droite Disconnected!");
  }

  sendtab[0] = '#';
  sendtab[1] = char(255 - adc0 / 25);  //les données doivent être placé ici ...
  sendtab[2] = char(moyenne);          // /25 car pas besoin de 6123 positions.
  sendtab[3] = char(adc2 / 25);
  sendtab[4] = char(adc6 / 25);
  sendtab[5] = char(adc7 / 25);
  sendtab[6] = sendtab[1] ^ sendtab[2] ^ sendtab[3] ^ sendtab[4] ^ sendtab[5];  //byte de controle d'intégrité de donnée


  for (char cptSend = 0; cptSend < NB_data; cptSend++) {
    SerialBT.print(sendtab[cptSend]);

#ifdef debug_ADC
    Serial.println(sendtab[cptSend]);
#endif
  }
}
//*******************************Initialisation**************************************
void setup() {

  delay(1000);
  M5.begin();

  //#ifdef USE_bluetooth
  init_bluetooth();
  //#endif
  secondWire.begin(I2C_SDA, I2C_SCL, (uint32_t)400000U);  // Initialisation du deuxième bus I2C avec les broches SDA et SCL définies
  init_screen(90, 90, 540, 960, 5);
  init_ads1();
  init_ads2();
  //************* Configuration de l'interruption du timer ************
  esp_timer_create_args_t timerArgs;       // crée une structure pour configurer le timer
  timerArgs.callback = &onTimer;           // Définition de la fonction de rappel
  timerArgs.arg = NULL;                    // rien de plus n'est passé à la fonction de rappel
  esp_timer_create(&timerArgs, &timer);    //Création du timer
  esp_timer_start_periodic(timer, 20000);  // Déclencher toutes les 20 ms
  //pinMode(PIN_G33, OUTPUT);
}
void loop() {

  /*if (Serial1.available()) {
              char Menu = Serial1.read();*/
  //canvas.drawString("le mode ", Menu, 20, 400);
  switch (Mode) {
    case MODE_MENU:
      canvas.clear();
      canvas.setTextSize(5);
      canvas.drawString("Choisissez le mode", 10, 100);
      canvas.drawString("Mode BT", 10, 200);
      canvas.drawString("Mode Manuel", 10, 400);
      canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
      if (M5.TP.available()) {                             // Vérifie si le pavé tactile est actif
        if (!M5.TP.isFingerUp()) {                         // Vérifie si un doigt est en contact avec l'écran
          M5.TP.update();                                  // Met à jour les informations du pavé tactile
          for (int i = 0; i < 2; i++) {                    // Parcourt les doigts détectés
            tp_finger_t FingerItem = M5.TP.readFinger(i);  // Lit les informations du doigt
            if ((FingerItem.x > 0 && FingerItem.x < 540) && (FingerItem.y > 0 && FingerItem.y < 960)) {
              // Vérifie si le toucher est dans la zone définie
              if (!inOptionsMenu) {
                if (FingerItem.y< 240 & FingerItem.y > 180) {
                  Mode = MODE_BT;
                } else if (FingerItem.y< 440 & FingerItem.y> 380) {
                  Mode = MODE_MANUEL;
                }
              }
            }
          }
        }
      }
      break;
    case MODE_MANUEL:
      canvas.clear();
      canvas.setTextSize(3);
      canvas.drawString("Welcom to manuel mode", 10, 100);
      canvas.drawString("Retour", 400, 800);
      canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
      if (M5.TP.available()) {                             // Vérifie si le pavé tactile est actif
        if (!M5.TP.isFingerUp()) {                         // Vérifie si un doigt est en contact avec l'écran
          M5.TP.update();                                  // Met à jour les informations du pavé tactile
          for (int i = 0; i < 2; i++) {                    // Parcourt les doigts détectés
            tp_finger_t FingerItem = M5.TP.readFinger(i);  // Lit les informations du doigt
            if ((FingerItem.x > 0 && FingerItem.x < 540) && (FingerItem.y > 0 && FingerItem.y < 960)) {
              // Vérifie si le toucher est dans la zone définie
              if (!inOptionsMenu) {
                Serial.printf("Finger ID:%d-->X: %d*C  Y: %d  Size: %d\r\n", FingerItem.id, FingerItem.x, FingerItem.y, FingerItem.size);

                if (FingerItem.y< 850 & FingerItem.y > 780) {
                  Mode = MODE_MENU;
                }
              }
            }
          }
        }
      }
      break;
    case MODE_BT:
      SerialBT.print("$");
      canvas.clear();
      canvas.setTextSize(3);
      canvas.drawString("Welcom to Bluetooth mode", 10, 50);
      canvas.drawString("Retour", 400, 800);
      static int cpt = 0;
      canvas.drawString("ADS1", 10, 130);
      canvas.drawString("A0: " + String(255 - adc0 / 25) + "mV", 10, 160);
      canvas.drawString("A1: " + String(adc1 / 25) + "mV", 10, 210);
      canvas.drawString("A2: " + String(adc2 / 25) + "mV", 10, 250);
      canvas.drawString("ADS2", 10, 310);
      canvas.drawString("A2: " + String(adc6 / 25) + "mV", 10, 360);
      canvas.drawString("A3: " + String(adc7 / 25) + "mV", 10, 400);

      canvas.drawString("Life CPT: " + String(cpt++), 10, 560);
      canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
      delay(20);                                           // Ajout d'un délai
      if (M5.TP.available()) {                             // Vérifie si le pavé tactile est actif
        if (!M5.TP.isFingerUp()) {                         // Vérifie si un doigt est en contact avec l'écran
          M5.TP.update();                                  // Met à jour les informations du pavé tactile
          for (int i = 0; i < 2; i++) {                    // Parcourt les doigts détectés
            tp_finger_t FingerItem = M5.TP.readFinger(i);  // Lit les informations du doigt
            if ((FingerItem.x > 0 && FingerItem.x < 540) && (FingerItem.y > 0 && FingerItem.y < 960)) {
              // Vérifie si le toucher est dans la zone définie
              if (!inOptionsMenu) {
                Serial.printf("Finger ID:%d-->X: %d*C  Y: %d  Size: %d\r\n", FingerItem.id, FingerItem.x, FingerItem.y, FingerItem.size);

                if (FingerItem.y< 850 & FingerItem.y > 780) {
                  Mode = MODE_MENU;
                }
              }
            }
          }
        }
      }


      break;
  }
}

void rectangle2() {
  canvas.drawRect(7, 390, 420, 100, 15);
}

// SerialBT.println('@');
//Serial.println('@');

/*if (M5.TP.available()) {
    if (!M5.TP.isFingerUp()) {
      M5.TP.update();
      canvas.fillCanvas(0);
      bool is_update = false;
      for (int i = 0; i < 2; i++) {
        tp_finger_t FingerItem = M5.TP.readFinger(i);
        if ((point[i][0] != FingerItem.x) || (point[i][1] != FingerItem.y)) {
          is_update = true;
          point[i][0] = FingerItem.x;
          point[i][1] = FingerItem.y;
          canvas.fillRect(FingerItem.x - 50, FingerItem.y - 50, 100,
                          100, 15);
          Serial.printf("Finger ID:%d-->X: %d*C  Y: %d  Size: %d\r\n", FingerItem.id, FingerItem.x, FingerItem.y, FingerItem.size);
          canvas.drawString("X: " + String(FingerItem.x) + " "
                                                           "Y:"
                              + String(FingerItem.y),
                            10, 60);
        }
      }
      if (is_update) {
        canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
      }
    }
  }*/