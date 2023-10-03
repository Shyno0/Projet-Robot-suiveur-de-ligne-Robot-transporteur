/*Attribution de noms aux PINs*/

// Moteur Gauche
#define PIN_LM 6
// Moteur Droit
#define PIN_RM 5

// Capeur de masse
#define PIN_WS 3
// Capteur de lumière gauche
#define PIN_LS 19
// Capteur de lumière milieu
#define PIN_MS 20 
// Capteur de lumière droit
#define PIN_RS 21 

// LED capteur gauche
#define LED_LS 8
// LED capteur milieu
#define LED_MS 7
// LED capteur droit
#define LED_RS 4
// LED d'erreur
#define ERROR_LED 2

// Creations de variables

bool point = 0; // Booléen - Valeur du point actuel (A ou B, 0 ou 1)
bool weight_sensor; // Booléen - Valeur détecté par le capteur de masse
unsigned char middle_sensor, left_sensor, right_sensor; // Variable de stockage des valeurs de luminosité détectés par les capteurs
const unsigned char sensibility = 60; // Constante - Valeur de la sensibilité entre lumière/ombre
unsigned char vel1 = 100, vel2, vel3;       // Constantes - Valeurs des vitesses prédéfinies
char i; // Variable pour compteur

void setup()
{
  vel2 = (vel1*1.2); // Defini la vitesse 2 en fonction de la vitesse 1 (prédefinie) et un multiplicateur
  vel3 = (vel1*1.3); // Defini la vitesse 3 en fonction de la vitesse 1 (prédefinie) et un multiplicateur
  
  //Tests (display)
  Serial.begin(9600);
  // Declare tous les PINs des moteurs en sortie
  pinMode(PIN_LM, OUTPUT); // Déclare Moteur Gauche
  pinMode(PIN_RM, OUTPUT); // Déclare Moteur Droit
  //pinMode(ERROR_LED, OUTPUT);

  // Declare tous les PINs des capteurs en entrée
  pinMode(PIN_WS, INPUT); // Déclare Capteur de Masse
  pinMode(PIN_LS, INPUT); // Déclare Capteur Gauche
  pinMode(PIN_MS, INPUT); // Déclare Capteur Milieu
  pinMode(PIN_RS, INPUT); // Déclare Capteur Droit

  // Declare tous les PINs des LEDs en sortie
  pinMode(LED_LS, OUTPUT);
  pinMode(LED_MS, OUTPUT);
  pinMode(LED_RS, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

// Programme principal [boucle]
void loop()
{
  read_measurements(); // Lit toutes les mesures de tous les capteurs
  
  /*Le robot se deplace du point A au point B avec une charge*/
  if(point == 0) // Si le robot se trouve au point A
  {
    if(weight_sensor == 0) // Si le capteur capte une charge
    {
      for (i=30; i>0; i--)   // Attendre 3s en verifiant que la charge est toujours presente
      {
        delay(100); // Attendre 100ms
        read_measurements();
        if(weight_sensor != 0) {break;} // Si le capteur ne capte plus la charge, alors sort de la boucle
      }
      if(i == 0)
      {
        forward();   // Avance
        delay(4000);  // Attendre 4s
        stop();      // Stop
        read_measurements(); 
        while(middle_sensor<=sensibility || left_sensor<=sensibility || right_sensor<=sensibility) // Tant que l'un des 3 capteur voit de la lumière
        {
          if(weight_sensor != 0) // Si le capteur ne capte plus la charge, alors stoppe le robot
          {
            stop();
            error_LED(0);
          }
          else // Sinon
          {
            movements(); // Mets en movements le robot
          }
          read_measurements();
        }
        stop();
        while(weight_sensor == 0) // Tant que le capteur capte une charge
        {
          // Ne rien faire/Attendre action humaine
          read_measurements();
        }
        point = 1;
      }
    }
  }

  /*Le robot se deplace du point B au point A sans charge*/
  else if(point == 1) // Si le robot se trouve au point B
  {
    if(weight_sensor =! 0) // Si le capteur capte une charge
    {
      for (i=30; i>0; i--)   // Attendre 3s en verifiant que la charge n'est pas presente
      {
        delay(100); // Attendre 100ms
        read_measurements();
        if(weight_sensor == 0) {break;} // Si le capteur capte une charge, alors sort de la boucle
      }
      if(i == 0)
      {
        forward();   // Avance
        delay(4000);  // Attendre 4s
        stop();      // Stop
        read_measurements(); 
        while(middle_sensor<=sensibility || left_sensor<=sensibility || right_sensor<=sensibility) // Tant que l'un des 3 capteur voit de la lumière
        {
          if(weight_sensor == 0) // Si le capteur capte une charge, alors stoppe le robot
          {
            stop();
            error_LED(0);
          }
          else // Sinon
          {
            movements(); // Mets en movements le robot
          }
          read_measurements();
        }
        stop();
        point = 0;
      }
    }
  }
}

// Sous programme : Lecture des PINs (Capteurs)
void read_measurements()
{
  weight_sensor = digitalRead(PIN_WS); // Lecture de la broche 3 (Capteur de masse)
  left_sensor = analogRead(PIN_LS);   // Lecture de la broche 21 (Capteur d'intensité lumineuse)
  middle_sensor = analogRead(PIN_MS); // Lecture de la broche 20 (Capteur d'intensité lumineuse)
  right_sensor = analogRead(PIN_RS);  // Lecture de la broche 19 (Capteur d'intensité lumineuse)
  if(left_sensor>sensibility){digitalWrite(LED_LS, HIGH);} else{digitalWrite(LED_LS, LOW);}   // Si le capteur gauche détecte la bande noire alors allume la LED gauche, sinon elle reste eteinte
  if(middle_sensor>sensibility){digitalWrite(LED_MS, HIGH);} else{digitalWrite(LED_MS, LOW);} // Si le capteur du milieu détecte la bande noire alors allume la LED du milieu, sinon elle reste eteinte
  if(right_sensor>sensibility){digitalWrite(LED_RS, HIGH);} else{digitalWrite(LED_RS, LOW);}  // Si le capteur droit détecte la bande noire alors allume la LED droite, sinon elle reste eteinte
}

// Sous programme : Affichage des meusures (Capteurs)
void display_measurments()
{
  Serial.print("Capteur masse  : ");  
  Serial.print(weight_sensor);
  Serial.print("\nCapteur gauche  : ");  
  Serial.print(left_sensor);
  Serial.print("\nCapteur milieu  : ");  
  Serial.print(middle_sensor);
  Serial.print("\nCapteur droit  : ");  
  Serial.print(right_sensor);
  Serial.print("\n\n");
}

// Sous programme : Affiche une variable au choix
void display(unsigned int variable)
{
  Serial.print("\nValeur:");
  Serial.print(variable);
}

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
  PIN_control(PIN_LM, vel1);
  PIN_control(PIN_RM, vel2);
}

void turn_right()
{
  // Tourne à droite
  PIN_control(PIN_LM, vel2);
  PIN_control(PIN_RM, vel1);
}

void turn_left2()
{
  // Tourne à gauche
  PIN_control(PIN_LM, vel1);
  PIN_control(PIN_RM, vel3);
}

void turn_right2()
{
  // Tourne à droite
  PIN_control(PIN_LM, vel3);
  PIN_control(PIN_RM, vel1);
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
  if(left_sensor<=sensibility && middle_sensor>sensibility && right_sensor<=sensibility)
  {
    error_LED(0);
    forward();
  }

  // Si détecte de la lumière sur le capteur gauche + milieu
  else if(left_sensor>sensibility && middle_sensor>sensibility && right_sensor<=sensibility)
  {
    error_LED(0);
    turn_left();
  }

  // Si détecte de la lumière sur le capteur droit + milieu
  else if(left_sensor<=sensibility && middle_sensor>sensibility && right_sensor>sensibility)
  {
    error_LED(0);
    turn_right();
  }
    
  // Si détecte de la lumière sur le capteur gauche
  else if(left_sensor>sensibility && middle_sensor<=sensibility && right_sensor<=sensibility)
  {
    error_LED(0);
    turn_left2();
  }

  // Si détecte de la lumière sur le capteur droit
  else if(left_sensor<=sensibility && middle_sensor<=sensibility && right_sensor>sensibility)
  {
    error_LED(0);
    turn_right2();
  }

  // Sinon
  else
  {
    stop();
  }

  delay(20);
}

// Sous programme : Defini l'état de la LED d'erreurs
void error_LED(bool state)
{
  if(state != 0)
  {
    digitalWrite(ERROR_LED, HIGH);
  }
  else
  {
    digitalWrite(ERROR_LED, state);
  }
} 
