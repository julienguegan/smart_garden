#include <Arduino.h>

// Définition des pins pour les pompes
#define PUMP_1 32
#define PUMP_2 13

// Durée d'activation de la pompe en millisecondes
const int PUMP_DURATION = 1000;  // 1 seconde

void setup() {
    // Initialisation de la communication série
    Serial.begin(9600);
    Serial.println("Test des pompes - Appuyez sur :");
    Serial.println("'1' pour activer la pompe 1");
    Serial.println("'2' pour activer la pompe 2");

    // Configuration des pins des pompes en sortie
    pinMode(PUMP_1, OUTPUT);
    pinMode(PUMP_2, OUTPUT);

    // S'assurer que les pompes sont éteintes au démarrage
    digitalWrite(PUMP_1, LOW);
    digitalWrite(PUMP_2, LOW);
}

void activatePump(int pumpPin) {
    digitalWrite(pumpPin, HIGH);
    Serial.print("Pompe sur pin ");
    Serial.print(pumpPin);
    Serial.println(" activée");
    
    delay(PUMP_DURATION);
    
    digitalWrite(pumpPin, LOW);
    Serial.print("Pompe sur pin ");
    Serial.print(pumpPin);
    Serial.println(" désactivée");
}

void loop() {
    // Vérifier si des données sont disponibles sur le port série
    if (Serial.available() > 0) {
        // Lire le caractère reçu
        char input = Serial.read();
        
        switch (input) {
            case '1':
                Serial.println("Commande reçue : activation pompe 1");
                activatePump(PUMP_1);
                break;
                
            case '2':
                Serial.println("Commande reçue : activation pompe 2");
                activatePump(PUMP_2);
                break;
                
            default:
                // Ignorer les autres caractères
                break;
        }
        
        // Vider le buffer série
        while(Serial.available() > 0) {
            Serial.read();
        }
    }
}
