Les commandes de lecture

Pour info, je ne liste que les commandes "utiles" à des fins de contrôle/domotisation même si d'autres commandes existent.

GET+LABL : Obtient le nom donné au poêle
GET+STDT : Obtient les informations sur la CBox, les configuration réseau WiFi et LAN et enfin la version et date du firmware du poêle ainsi que le type de pellet et la configuration du poêle (1 normal ou 2 chrono-thermostat extérieur
GET+FWST : Obtient l'état du Firmware (???)
GET+TMPS : Obtient la température relevée par la sonde d'ambiance du poêle (nommée TMP_ROOM_WATER)
GET+FAND : Obtient la configuration de ventilation et la vitesse des ventilateurs
GET+SETP : Obtient la consigne actuelle du poêle
GET+STAT : Obtient le status du poêle.
0: OFF
1: OFF TIMER
2: TESTFIRE
3: HEATUP
4: FUELIGN
5: IGNTEST
6: BURNING
9: COOLFLUID
10: FIRESTOP
11: CLEANFIRE
12: COOL

241: CHIMNEY ALARM
243: GRATE ERROR
244: NTC2 ALARM
245: NTC3 ALARM
247: DOOR ALARM
248: PRESS ALARM
249: NTC1 ALARM
250: TC1 ALARM
252: GAS ALARM
253: NOPELLET ALARM


GET+TIME : Obtient la date/heure du poêle et le jour de la semaine (1=lundi, 2=mardi, etc., ...)
GET+MDVE : Obtient le model du poêle, la version et la date du firmware
GET+CHRD : Obtient les différentes programmations horaires et les programmes des jours de la semaine
EXT+ADRD+2066+1 : Obtient le nombre d'allumage du poêle
EXT+ADRD+206A+1 : Obtient le nombre d'heure durant laquelle le poêle a été alimenté électriquement
EXT+ADRD+2070+1 : Obtient le nombre d'heure de chauffe total
EXT+ADRD+2076+1 : Obtient le nombre d'heure de chauffe depuis le dernier entretien
EXT+ADRD+207C+1 : Obtient le nombre d'allumage manqué (à confirmer)
EXT+ADRD+207A+1 : Obtient le nombre d'erreur pour cause de surchauffe


Les commandes d'écriture/Contrôle du poêle


CMD+ON : Allumage du Poêle
CMD+OFF : Extinction du Poêle
SETP+20 : réglage de la consigne à la température désirée
RFAN+1 : réglage de la vitesse de ventilation (de 0 à 5; 0 étant le mode auto)
POWR+1 : réglage de la puissance de chauffe (de 1 à 5)
F1: Internal Fan
Speed        "F1RPM":   1506
Power Supply "F1V":     100

F2: Room fan
Consigne     "F2L":  0   1   2   3   4   5   6   7
???          "F2LF": 0   0   0   0   0   0   1   2
Power Supply "F2V":  0   150 160 170 180 200 165 156


Feeder: "FDR": 0.9 DPT9.025 3/0/14

ON/OFF    3/0/5
"F2L":  
"F2LF":
"F2V":    3/0/11
"F1V":    3/0/12
"F1RPM":  3/0/13

GET+ALLS still unknown


    "DATA": {
        "DP": 1189,
        "DPT": 1170,
        "FANLMINMAX": [
            0,
            5,
            0,
            1,
            0,
            1
        ],
        "IN": 15,
        "LSTATUS": 6,
        "OUT": 6,
    },

