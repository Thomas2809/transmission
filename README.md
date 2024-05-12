# La main robotique

## transmission_gant_main

 ## Introduction:
 Comme projet de fin d’études, j’ai décidé de créer une main robotique controle à distance. 
 
Mon projet ce compose en deux parties. 

La première partie concernée est le gant Bluetooth. 
Dans ce gant on retrouvve 5 capteurs de flexion (flex sensor) qui sont connectés en série à une résistance de 8k2Ω. Ces cinq résistances sont soudées sur deux cartes ADS qui sont reliées ensemble. 
L’ensemble est attaché à mon bras à l’aide de deux pièces 3D que j’ai personnellement dessinées en 3D et imprimées avec mon imprimante 3D. 

La deuxième partie, nous avons une main robotisée qui contient 5 servos moteurs. 
Ceux-ci sont reliés à un microcontrôleur ESP32 qui en Bluetooth, qui va récupèrer les informations nécessaires pour faire fonctionner la main. 
A l’entrée de la carte ESP32, il y a également 8 encodeurs qui me permettent de faire lever ou abaisser les doigts de la même façon que le gant. 

## Tout le matériel nécessaire pour réaliser le projet:

Une feather ESP32
Une M5STACK PAPER
Une main robotique 
Des encodeurs 
Des capteurs de flexion 
Module ADS1115 
Un PCB sur le quel on retrouve des polyswitch (fusible réarmable), des résistanceq de 220 ohm, un intérupteur pour éteindre les servos moteurs et des connecteurs pour les servos et l'encodeur.

## Shéma de principe de la main robotique:


