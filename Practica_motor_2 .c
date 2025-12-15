// =================================================================
// 1. DEFINICIÓN DE PINES
// =================================================================

// --- MOTORES ---
// Lado Izquierdo (M1 y M3)
int EntradaA1 = 2; 
int EntradaA2 = 3;
int EntradaC1 = 6; 
int EntradaC2 = 7;

// Lado Derecho (M2 y M4)
int EntradaB1 = 4; 
int EntradaB2 = 5;
int EntradaD1 = 8; 
int EntradaD2 = 9;

// --- BOTONES (ENTRADAS) ---
int btnAdelante = 10;
int btnAtras    = 11;
int btnIzq      = 12;
int btnDer      = 13;

// --- LEDS INDICADORES (SALIDAS) ---
int ledAdelante = A0;
int ledAtras    = A1;
int ledIzq      = A2;
int ledDer      = A3;

// =================================================================
// 2. FUNCIÓN SETUP
// =================================================================

void setup() {
  
  // 1. Configurar pines de motores como SALIDA
  pinMode(EntradaA1, OUTPUT); pinMode(EntradaA2, OUTPUT);
  pinMode(EntradaB1, OUTPUT); pinMode(EntradaB2, OUTPUT);
  pinMode(EntradaC1, OUTPUT); pinMode(EntradaC2, OUTPUT);
  pinMode(EntradaD1, OUTPUT); pinMode(EntradaD2, OUTPUT);

  // 2. Configurar BOTONES como ENTRADA PULLUP
  // (Recuerda: LOW significa presionado)
  pinMode(btnAdelante, INPUT_PULLUP);
  pinMode(btnAtras,    INPUT_PULLUP);
  pinMode(btnIzq,      INPUT_PULLUP);
  pinMode(btnDer,      INPUT_PULLUP);

  // 3. Configurar LEDS como SALIDA
  pinMode(ledAdelante, OUTPUT); 
  pinMode(ledAtras,    OUTPUT);
  pinMode(ledIzq,      OUTPUT);
  pinMode(ledDer,      OUTPUT);

  // 4. Inicialización de Seguridad (Todo apagado)
  pararMotores();
  apagarTodosLeds();
}

// =================================================================
// 3. FUNCIÓN LOOP
// =================================================================

void loop() {
  
  // Usamos "else if" para evitar conflictos si se presionan dos botones a la vez.
  
  // --- AVANZAR (Botón 10) ---
  if (digitalRead(btnAdelante) == LOW) {
    moverIzquierda(true);  // Izq Adelante
    moverDerecha(true);    // Der Adelante
    encenderLed(ledAdelante);
  }
  
  // --- RETROCEDER (Botón 11) ---
  else if (digitalRead(btnAtras) == LOW) {
    moverIzquierda(false); // Izq Atras
    moverDerecha(false);   // Der Atras
    encenderLed(ledAtras);
  }
  
  // --- GIRAR IZQUIERDA (Botón 12) ---
  // Giro tipo tanque: Izquierda retrocede, Derecha avanza
  else if (digitalRead(btnIzq) == LOW) {
    moverIzquierda(false); // Izq Atras
    moverDerecha(true);    // Der Adelante
    encenderLed(ledIzq);
  }
  
  // --- GIRAR DERECHA (Botón 13) ---
  // Giro tipo tanque: Izquierda avanza, Derecha retrocede
  else if (digitalRead(btnDer) == LOW) {
    moverIzquierda(true);  // Izq Adelante
    moverDerecha(false);   // Der Atras
    encenderLed(ledDer);
  }
  
  // --- NINGÚN BOTÓN PRESIONADO ---
  else {
    pararMotores();
    apagarTodosLeds();
  }
}

// =================================================================
// 4. FUNCIONES AUXILIARES
// =================================================================

void pararMotores() {
  digitalWrite(EntradaA1, LOW); digitalWrite(EntradaA2, LOW);
  digitalWrite(EntradaC1, LOW); digitalWrite(EntradaC2, LOW);
  digitalWrite(EntradaB1, LOW); digitalWrite(EntradaB2, LOW);
  digitalWrite(EntradaD1, LOW); digitalWrite(EntradaD2, LOW);
}

// Función para apagar todos los LEDs de golpe
void apagarTodosLeds() {
  digitalWrite(ledAdelante, LOW);
  digitalWrite(ledAtras, LOW);
  digitalWrite(ledIzq, LOW);
  digitalWrite(ledDer, LOW);
}

// Función para encender solo un LED y asegurar que los otros se apaguen
void encenderLed(int ledPin) {
  apagarTodosLeds(); // Primero apaga todos por seguridad
  digitalWrite(ledPin, HIGH); // Enciende el indicado
}

// Mueve los motores del Lado IZQUIERDO (M1, M3)
// bool avance: true = ir hacia adelante, false = ir hacia atras
void moverIzquierda(bool avance) {
  if (avance) {
    // Adelante
    digitalWrite(EntradaA1, HIGH); digitalWrite(EntradaA2, LOW); 
    digitalWrite(EntradaC1, HIGH); digitalWrite(EntradaC2, LOW); 
  } else {
    // Atras (Invertimos polaridad: LOW/HIGH)
    digitalWrite(EntradaA1, LOW);  digitalWrite(EntradaA2, HIGH); 
    digitalWrite(EntradaC1, LOW);  digitalWrite(EntradaC2, HIGH); 
  }
}

// Mueve los motores del Lado DERECHO (M2, M4)
// NOTA: Tu lado derecho tiene cableado invertido en el original (LOW/HIGH para avanzar)
void moverDerecha(bool avance) {
  if (avance) {
    // Adelante (Según tu lógica original)
    digitalWrite(EntradaB1, LOW);  digitalWrite(EntradaB2, HIGH); 
    digitalWrite(EntradaD1, LOW);  digitalWrite(EntradaD2, HIGH); 
  } else {
    // Atras (Invertimos polaridad: HIGH/LOW)
    digitalWrite(EntradaB1, HIGH); digitalWrite(EntradaB2, LOW); 
    digitalWrite(EntradaD1, HIGH); digitalWrite(EntradaD2, LOW); 
  }
}