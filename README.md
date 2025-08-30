# smart_garden

Projet en arduino où j'ai construit un jardin connecté en utilisant un ESP32, des sondes d'humidité et des petites pompes à eau. J'ai également utilisé la librairie blynk pour construire facilement une application mobile et stocker les données dans un espace cloud.
Bon y'a encore des petits bug niveau hardware qu'il faut que je regarde plus en détails

![MicrosoftTeams-image (37)](https://github.com/julienguegan/smart_garden/assets/45081450/cda37684-1d80-44c9-aa63-bd82c95ccbbb)

![MicrosoftTeams-image (36)](https://github.com/julienguegan/smart_garden/assets/45081450/b144a38a-7671-4c3b-9780-f8b499abbdba)

## Configuration

### Configuration WiFi

Pour configurer le mot de passe WiFi de manière sécurisée, vous avez deux options :

1. **Utiliser une variable d'environnement** (recommandé) :
   ```bash
   export WIFI_PASSWORD="votre_mot_de_passe"
   pio run
   ```

2. **Configuration locale** :
   - Copiez le fichier `platformio.ini.example` en `platformio.ini`
   - Modifiez la valeur de `WIFI_PASSWORD` dans le fichier
   
Note : Le fichier `platformio.ini` est ignoré par git pour éviter de commiter des informations sensibles.
