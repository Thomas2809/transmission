# La main robotique

## transmission_gant_main

 ## Introduction
 Comme projet de fin d’études, j’ai décidé de créer une main robotique controlé à distance. 
 Ce projet peut être utile dans le domaine chirurgical ou si il s'agit de manipuler des matières phosphorescente par exemple.
 
Mon projet ce compose en deux parties:

La première partie concernée est le gant Bluetooth. 
Dans ce gant on retrouvve 5 capteurs de flexion (flex sensor) qui sont connectés en série à une résistance de 8k2Ω. Ces cinq résistances sont soudées sur deux cartes ADS qui sont reliées l'une à l'autre. 
L’ensemble est attaché à mon bras à l’aide de deux pièces 3D que j’ai personnellement dessinées en 3D et imprimées avec mon imprimante 3D. 

La deuxième partie, nous avons une main robotisée qui contient 5 servos moteurs. 
Ceux-ci sont reliés à un microcontrôleur ESP32 qui en Bluetooth, qui va récupèrer les informations nécessaires pour faire fonctionner la main. 
A l’entrée de la carte ESP32, il y a également 8 encodeurs qui me permettent de faire lever ou abaisser les doigts de la même façon que le gant. 

## Tout le matériel nécessaire pour réaliser le projet

Une feather ESP32
Une M5STACK PAPER
Une main robotique 
Des encodeurs 
Des capteurs de flexion 
Module ADS1115 
Un PCB sur le quel on retrouve des polyswitch (fusible réarmable), des résistanceq de 220 ohm, un intérupteur pour éteindre les servos moteurs et des connecteurs pour les servos et l'encodeur.

# Bibliothèque nécessaire

Bibliothèque utilisée pour les servos moteurs : 
•	https://github.com/jkb-git/ESP32Servo

Bibliothèque utilisée pour le m5rotate: 
•	https://github.com/RobTillaart/M5ROTATE8

Bibliothèque utilisée pour les ADC: 
•	https://github.com/DFRobot/DFRobot_ADS1115/blob/master/DFRobot_ADS1115.h

Bibliothèque utilisée pour le M5EPD: 
•	https://github.com/m5stack/M5EPD 

 Bibliothèque utilisée pour le Bluetooth : 
•	https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/

# Partie mécanique

## Shéma de principe de la main robotique   

![image](https://github.com/Thomas2809/transmission/assets/166612110/11a5ead6-651d-49cb-8e9f-05609cc512b8)              

## Schéma de principe du gant 

 ![image](https://github.com/Thomas2809/transmission/assets/166612110/93310c57-e7f2-4853-92e2-c30dc33eae3a)

## Shéma de principe du PCB

![image](https://github.com/Thomas2809/transmission/assets/166612110/8129181a-622d-492e-846e-ad93d746f215)


