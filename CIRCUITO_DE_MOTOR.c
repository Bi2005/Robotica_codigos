const int ENA = 9;   
const int IN1 = 8;    
const int IN2 = 7;    

const int botonPin = 2;
const int ledVerde = 3;
const int ledRojo = 4;
const int botonReversoPin = 10;    
const int ledDireccionPin = 5;    

bool motorActivo = false;
bool direccionReversa = false;    
int velocidad = 200;  

void setup() {
  // Configurar pines del L293N
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Configurar interface de usuario
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  
  // Configurar nuevos pines
  pinMode(botonReversoPin, INPUT_PULLUP);
  pinMode(ledDireccionPin, OUTPUT);
  
  // Inicializar comunicación serial
  Serial.begin(9600);
  Serial.println("Sistema de control motor DC con L293N iniciado");
  
  // Estado inicial - motor detenido
  detenerMotor();
  // Inicializar LED de dirección (Adelante por defecto)
  digitalWrite(ledDireccionPin, LOW); // LOW (apagado) para adelante
}

void loop() {
  // Leer estado del pulsador principal (Activar/Detener)
  if (digitalRead(botonPin) == LOW) {
    delay(50);  // Anti-rebote simple
    if (digitalRead(botonPin) == LOW) {
      toggleMotor();
      while(digitalRead(botonPin) == LOW);  // Esperar a soltar botón
    }
  }
  
  // Leer estado del pulsador de reverso (Cambio de dirección)
  if (digitalRead(botonReversoPin) == LOW) {
    delay(50);  // Anti-rebote simple
    if (digitalRead(botonReversoPin) == LOW) {
      cambiarDireccion();
      while(digitalRead(botonReversoPin) == LOW);  // Esperar a soltar botón
    }
  }
}

void toggleMotor() {
  if (motorActivo) {
    detenerMotor();
  } else {
    activarMotor();
  }
}

// --- Funciones de Control de Motor ---

void activarMotor() {
  // Configurar dirección basada en la variable
  if (direccionReversa) {
    // Reversa
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(ledDireccionPin, HIGH); // LED ENCEDIDO para reversa
    Serial.println(" MOTOR ACTIVADO - Giro hacia REVERSA");
  } else {
    // Adelante
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(ledDireccionPin, LOW); // LED APAGADO para adelante
    Serial.println(" MOTOR ACTIVADO - Giro hacia ADELANTE");
  }

  // Activar motor con velocidad
  analogWrite(ENA, velocidad);  // Control PWM
  
  // Feedback visual principal
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledRojo, LOW);
  
  // Feedback serial
  Serial.print("Velocidad PWM: ");
  Serial.println(velocidad);
  
  motorActivo = true;
}

void detenerMotor() {
  // Detener motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  
  // Feedback visual principal
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, HIGH);
  
  // Feedback serial
  Serial.println(" MOTOR DETENIDO");
  
  motorActivo = false;
}

void cambiarDireccion() {
  // Invertir el estado de la dirección
  direccionReversa = !direccionReversa; 
  
  Serial.print("Dirección cambiada a: ");
  Serial.println(direccionReversa ? "REVERSA" : "ADELANTE");

  // Si el motor está activo, aplicamos el cambio de inmediato.
  if (motorActivo) {
    activarMotor(); // Reconfigura IN1/IN2 y el ledDireccionPin
  } else {
    // Si el motor está detenido, solo actualizamos el LED de dirección
    // y el mensaje serial, para que esté listo al activarse.
    digitalWrite(ledDireccionPin, direccionReversa ? HIGH : LOW);
  }
}
