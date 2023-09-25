/*Attribution de noms aux PINs*/

// Moteur Gauche
#define PIN_LM 6
// Moteur Droit
#define PIN_RM 5

// Capteur de lumière milieu
#define PIN_MS A0 
// Capteur de lumière droit
#define PIN_RS A1 
// Capteur de lumière gauche
#define PIN_LS A2
// Cpateur de masse
#define PIN_WS 3

//#define ERROR_LED --

// Creations de variables

bool weight_sensor; // Booléen - Valeur détecté par le capteur de masse
unsigned char middle_sensor, left_sensor, right_sensor; // Variable de stockage des valeurs de luminosité détectés par les capteurs
const unsigned char sensibility = 10;       // Constante - Valeur de la sensibilité entre lumière/ombre
const unsigned char vel1 = 120, vel2 = 100; // Constantes - Valeurs des vitesses prédéfinies

// Sous programme : Mise en marche ou arrêt des PINs (moteurs) et ajustement de vitesse
void PIN_control(unsigned char PIN, unsigned char velocity) // PIN_control(PIN, velocity);
{
  if(velocity != 0)
  {
    digitalWrite(PIN, HIGH);
    analogWrite(PIN, velocity);
  }
  else
  {
    digitalWrite(PIN, LOW);
  }
}

// Sous programme : Lecture des PINs (Capteurs)
void read_measurements()
{
  weight_sensor = analogRead(PIN_WS); // Lecture de la broche D3 (Capteur de poids)
  middle_sensor = analogRead(PIN_MS); // Lecture de la broche A0 (Capteur d'intensité lumineuse)
  right_sensor = analogRead(PIN_RS);  // Lecture de la broche A1 (Capteur d'intensité lumineuse)
  left_sensor = analogRead(PIN_LS);   // Lecture de la broche A2 (Capteur d'intensité lumineuse)
}

// Sous programmes : Movements du robot
void forward()
{
  // Va tout droit
  PIN_control(PIN_LM, vel1);
  PIN_control(PIN_RM, vel1);
}

void turn_left()
{
  // Tourne à gauche
  PIN_control(PIN_LM, vel2);
  PIN_control(PIN_RM, vel1);
}

void turn_right()
{
  // Tourne à droite
  PIN_control(PIN_LM, vel1);
  PIN_control(PIN_RM, vel2);
}

void stop()
{
  // Arrêt
  PIN_control(PIN_LM, 0);
  PIN_control(PIN_RM, 0);
}

// Sous programme : Movements du robot (pathfinding) + conditions [boucle]
void movements()
{
  // Si détecte aucune lumière sur les 2 capteurs d'extermité mais seulment au milieu
  if(middle_sensor<sensibility && right_sensor>=sensibility && left_sensor>=sensibility)
  {
    //digitalWrite(ERROR_LED, LOW);
    forward();
  }

  // Si détecte de la lumière sur le capteur droit + milieu
  else if(middle_sensor<sensibility && left_sensor<sensibility && right_sensor>=sensibility)
  {
    //digitalWrite(ERROR_LED, LOW);
    turn_left();
  }

  // Si détecte de la lumière sur le capteur gauche + milieu
  else if(middle_sensor<sensibility && left_sensor>=sensibility && right_sensor<sensibility)
  {
    //digitalWrite(ERROR_LED, LOW);
    turn_right();
  }

  // Sinon
  else
  {
    stop();
    //digitalWrite(ERROR_LED, HIGH);
  }

  delay(100);
}

void setup()
{
  // Declare tous les PINs des moteurs en sortie
  pinMode(PIN_LM, OUTPUT); // Déclare Moteur Gauche
  pinMode(PIN_RM, OUTPUT); // Déclare Moteur Droit
  //pinMode(ERROR_LED, OUTPUT);

  // Declare tous les PINs des capteurs en entrée
  pinMode(PIN_MS, INPUT); // Déclare Capteur Milieu
  pinMode(PIN_RS, INPUT); // Déclare Capteur Droit
  pinMode(PIN_LS, INPUT); // Déclare Capteur Gauche
  pinMode(PIN_WS, INPUT); // Déclare Capteur de Masse
}

// Programme principal [boucle]
void loop()
{
  read_measurements(); // Lit toutes les mesures de tous les capteurs

  if(weight_sensor == 1) // Si le capteur capte une charge
  {
    /*Le robot se deplace du point A au point B avec une charge*/

    delay(3000); // Attendre 3s
    forward();   // Avance
    delay(500);  // Attendre 0.5s
    stop();      // Stop
    read_measurements(); 
    while((middle_sensor>=sensibility || left_sensor>=sensibility || right_sensor>=sensibility) && (weight_sensor == 1)) // Tant que l'un des 3 capteur voit de la lumière + porte une charge
    {
      movements(); // Mets en movements le robot
      read_measurements();
    }
    stop();
    while(weight_sensor == 1) // Tant que le capteur capte une charge
    {
      // Ne rien faire/Attendre action humaine
      read_measurements();
    }

    /*Le robot se deplace du point B au point A sans charge*/

    delay(3000);
    forward();
    delay(500);
    stop();
    read_measurements(); 
    while(middle_sensor>=sensibility || left_sensor>=sensibility || right_sensor>=sensibility) // Tant que l'un des 3 capteur voit de la lumière
    {
      movements(); // Mets en movements le robot
      read_measurements();
    }
    stop();
  }
}
