
#include "BluetoothSerial.h"
#include <M5EPD.h>
#include "DFRobot_ADS1115.h"

extern M5EPD_Canvas canvas;
extern DFRobot_ADS1115 ads1;
extern DFRobot_ADS1115 ads2;
extern BluetoothSerial SerialBT;

void init_bluetooth(void);
void init_screen(int sens_Sc,int sens_Tp, int largeur, int hauteur, int texte_size);
void init_ads(void);