# La transmission de la main robotique
 [![Static Badge](https://img.shields.io/badge/Transmission%20main%20robotis%C3%A9%20-61DBFB?style=for-the-badge&logo=Arduino&labelColor=red)](https://github.com/Thomas2809/transmission)
 # La réception de la main robotique
 [![Static Badge](https://img.shields.io/badge/Reception%20main%20robotis%C3%A9%20-61DBFB?style=for-the-badge&logo=Arduino&labelColor=red)](https://github.com/Daoud1190/Reception_moteur_main)
## Démonstration du projet
https://github.com/Thomas2809/transmission/assets/166612110/041ae254-5347-47f5-9a14-3c6a6fa173a5
## Table des matières
- [Introduction](#introduction)
- [Liste des composants](#liste-des-composants)
- [Les caractéristiques](#les-caractéristiques)
- [Bibliothèques nécessaires](#bibliothèques-nécessaires)
- [Partie mécanique](#partie-mécanique)
   - [Schéma de principe du gant](#schéma-de-principe-du-gant)
   - [Schéma de principe de la main robotique](#schéma-de-principe-de-la-main-robotique)
   - [Schéma de principe du PCB](#schéma-de-principe-du-PCB)
- [Schéma bloc](#schéma-bloc)      
  - [Gant](#Gant)
  - [Main](#Main)


## Introduction
Comme projet de fin d’études, j’ai décidé de créer une main robotique controlée à distance. 
Ce projet peut être utile dans le domaine chirurgical ou s'il s'agit de manipuler des matières phosphorescentes par exemple.

Le projet se compose en deux parties:

La première partie concernée est le gant Bluetooth. 
Dans ce gant, on retrouve 5 capteurs de flexion (flex sensor) qui sont connectés en série à une résistance de 8k2Ω. Ces cinq résistances sont soudées sur deux cartes ADS qui sont reliées l'une à l'autre. 
L’ensemble est attaché à mon bras à l’aide de deux pièces 3D que j’ai personnellement dessinées en 3D et imprimées avec mon imprimante 3D. 

La deuxième partie est constituée d'une main robotisée qui contient 5 servos moteurs. 
Ceux-ci sont reliés à un microcontrôleur ESP32 en Bluetooth qui va récupérer les informations nécessaires pour faire fonctionner la main.
À l’entrée de la carte ESP32, il y a également 8 encodeurs qui me permettent de faire lever ou abaisser les doigts de la même façon que le gant. 

## Liste des composants

- Une feather ESP32
- Une M5STACK PAPER
- Une main robotique 
- Des encodeurs 
- Des capteurs de flexion 
- Module ADS1115 
- Un PCB sur lequel on retrouve des polyswitches (fusibles réarmables), des résistances de 220 ohm, un interrupteur pour éteindre les servomoteurs et des connecteurs pour les servos et l'encodeur.

## Les caractéristiques

### Main :
- Alimentée en 5Vdc
- Contrôlée en Bluetooth (2,4GHz) via une Feather (ESP32)
- Autonomie à déterminer en fonction de la batterie
- 300 mA/doigts 
- Tension de fonctionnement des servomoteurs: de 5 à 6 V
- Protocole de contrôle : 0,5ms à 2,5ms pour des servomoteurs 180°
### Gant :
- Alimenté en 5VDC
- 5 capteurs de flexion commandés en 5VDC
- Autonomie +- 4h20
- Capacitée 1150mAh
- Contrôlé en Bluetooth (2,4GHz) via une Feather 

## Bibliothèques nécessaires

- [Servomoteurs](https://github.com/jkb-git/ESP32Servo)
- [M5rotate des encodeurs](https://github.com/RobTillaart/M5ROTATE8)
- [ADC](https://github.com/DFRobot/DFRobot_ADS1115/blob/master/DFRobot_ADS1115.h)
- [M5EPD](https://github.com/m5stack/M5EPD) 
- [Bluetooth](https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/)

## Partie mécanique

### Schéma de principe du gant 

![image](https://github.com/Thomas2809/transmission/assets/166612110/93310c57-e7f2-4853-92e2-c30dc33eae3a)

### Schéma de principe de la main robotique   

![image](https://github.com/Thomas2809/transmission/assets/166612110/11a5ead6-651d-49cb-8e9f-05609cc512b8)              

### Schéma de principe du PCB

![image](https://github.com/Thomas2809/transmission/assets/166612110/70e799e9-662f-41b8-b150-6f8a6a61cec8)

[lien easyeda](https://oshwlab.com/thomasgiarrizzo/sch-ma-final)

## Schéma bloc

### Gant

![image](https://github.com/Thomas2809/transmission/assets/166612110/563deee5-c57e-43f0-a7b9-ecd2851ca234)

### Main

![image](https://github.com/Thomas2809/transmission/assets/166612110/3fe65ea3-31b4-4b9d-a606-61c25c475a2b)
