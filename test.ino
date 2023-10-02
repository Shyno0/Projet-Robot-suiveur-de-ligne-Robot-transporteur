unsigned int cg,cm,cd;
unsigned long cgadd = 0,cmadd = 0,cdadd = 0;
const char LEDg = 4, LEDm = 7, LEDd = 8;
char i;
bool poids;

void setup()
{
  //Tests
  Serial.begin(9600);
  // Declare tous les PINs des moteurs en sortie
  pinMode(6, OUTPUT); // Déclare Moteur Gauche
  pinMode(5, OUTPUT); // Déclare Moteur Droit
  //pinMode(ERROR_LED, OUTPUT);

  // Declare tous les PINs des capteurs en entrée
  pinMode(A0, INPUT); // Déclare Capteur Gauche
  pinMode(A1, INPUT); // Déclare Capteur Milieu
  pinMode(A2, INPUT); // Déclare Capteur Droit
  pinMode(3, INPUT); // Déclare Capteur de Masse

  // Declare tous les PINs des LEDs en sortie
  pinMode(LEDg, OUTPUT);
  pinMode(LEDm, OUTPUT);
  pinMode(LEDd, OUTPUT);
}

// Programme principal [boucle]
void loop()
{
  poids = digitalRead(3); // Lecture de la broche 3 (Capteur de masse)

  cgadd = 0;
  cmadd = 0;
  cdadd = 0;

  for(i=50; i>0; i--)
  {
    cg = analogRead(A0);
    cm = analogRead(A1);
    cd = analogRead(A2);
    /*Serial.print(cgadd);
    Serial.print("\n\n");*/
    cmadd = cmadd+cm;
    cgadd = cgadd+cg;
    cdadd = cdadd+cd;
    delay(1);
  }
  cgadd = cgadd/50;
  cmadd = cmadd/50;
  cdadd = cdadd/50;
  
  if(cgadd<=25){
    digitalWrite(LEDg, HIGH);} 
  else{digitalWrite(LEDg, LOW);}
  if(cmadd<=25){
    digitalWrite(LEDm, HIGH);} 
  else{digitalWrite(LEDm, LOW);}
  if(cdadd<=25){
    digitalWrite(LEDd, HIGH);} 
  else{digitalWrite(LEDd, LOW);}

  Serial.print("Capteur masse  : ");  
  Serial.print(poids);
  Serial.print("\nCapteur gauche  : ");  
  Serial.print(cgadd);
  Serial.print("\nCapteur milieu  : ");  
  Serial.print(cmadd);
  Serial.print("\nCapteur droit  : ");  
  Serial.print(cdadd);
  Serial.print("\n\n");
  delay(200);
}
