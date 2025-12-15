
const int LED_ROJO = 13;   
const int LED_AMARILLO = 12; 
const int LED_VERDE = 11;   
const int LED_AZUL = 10;    


const int TIEMPO_VERDE = 5000;    
const int TIEMPO_AMARILLO = 2000;
const int TIEMPO_ROJO = 7000;     

void setup() {
  
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
}

void loop() {
  
  digitalWrite(LED_VERDE, HIGH);    
  digitalWrite(LED_ROJO, LOW);     
  digitalWrite(LED_AMARILLO, LOW); 
  digitalWrite(LED_AZUL, LOW);     
  delay(TIEMPO_VERDE);

  
  digitalWrite(LED_VERDE, LOW);    
  digitalWrite(LED_AMARILLO, HIGH);
  delay(TIEMPO_AMARILLO);


  digitalWrite(LED_AMARILLO, LOW); 
  digitalWrite(LED_ROJO, HIGH);    
  digitalWrite(LED_AZUL, HIGH);    
  delay(TIEMPO_ROJO);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_AZUL, LOW); 
    delay(500);
    digitalWrite(LED_AZUL, HIGH); 
    delay(500);
  }

 
  digitalWrite(LED_AZUL, LOW);
  delay(500);
}