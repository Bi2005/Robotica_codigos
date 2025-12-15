#include <LiquidCrystal.h>

// --- Configuración del LCD (RS, E, D4, D5, D6, D7) ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define LCD_COLS 16
#define LCD_ROWS 2

// --- Pines del sensor ultrasónico ---
const int PinTrig = 9;
const int PinEcho = 10;

// --- Parámetros físicos ---
const float VelSon = 34300.0;       // Velocidad del sonido en cm/s
const int numLecturas = 5;          // Tamaño del promedio móvil

// --- Variables para promedio móvil ---
float lecturas[numLecturas];
int indiceLectura = 0;
float sumaLecturas = 0;
bool promedioListo = false;
float ultimaLectura = 8.7;          // Valor inicial si el sensor falla

// --- Tabla de calibración: {distancia_cm, volumen_ml} ---
const int numPuntos = 15;
float tabla[numPuntos][2] = {
  {8.7,    20000}, {15.0, 19000}, {25.0, 17500}, {40.0, 15500},
  {60.0,   13000}, {85.0, 10500}, {115.0, 8000}, {150.0, 5500},
  {180.0,  4000}, {200.0, 3200}, {220.0, 2000}, {235.0, 1500},
  {245.0, 1000}, {250.0, 700}, {254.35, 500}
};

void setup() {
  Serial.begin(9600);
  pinMode(PinTrig, OUTPUT);
  pinMode(PinEcho, INPUT);

  for (int i = 0; i < numLecturas; i++) lecturas[i] = tabla[0][0];
  sumaLecturas = tabla[0][0] * numLecturas;

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Iniciando...");
  delay(1000);
  lcd.clear();

  Serial.println("=================================");
  Serial.println("  SISTEMA DE NIVEL ULTRASONICO  ");
  Serial.println("=================================");
  Serial.println("Esperando lecturas...");
  Serial.println();
}

void loop() {
  sumaLecturas -= lecturas[indiceLectura];
  float distancia = medirDistancia();
  lecturas[indiceLectura] = distancia;
  sumaLecturas += distancia;

  ultimaLectura = distancia;  // guardar última lectura válida
  indiceLectura++;
  if (indiceLectura >= numLecturas) {
    indiceLectura = 0;
    promedioListo = true;
  }

  float promedio = sumaLecturas / numLecturas;

  if (promedioListo) {
    float volumen = calcularVolumen(promedio);
    float porcentaje = (volumen / tabla[0][1]) * 100.0;

    // Mostrar en LCD y Serial
    mostrarLCD(volumen, porcentaje, promedio);
    mostrarSerial(volumen, porcentaje, promedio);
  } else {
    lcd.setCursor(0,0);
    lcd.print("Calculando...   "); // espacios para limpiar resto de línea
    lcd.setCursor(0,1);
    lcd.print("Lectura " + String(indiceLectura) + "       ");
  }

  delay(800);
}

// --- Función para medir distancia ---
float medirDistancia() {
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrig, LOW);

  unsigned long duracion = pulseIn(PinEcho, HIGH, 40000); // timeout 40ms
  if (duracion == 0) return ultimaLectura; // usar última lectura válida

  float distancia = (duracion * 0.000001 * VelSon) / 2.0;
  return distancia;
}

// --- Interpolación lineal para calcular volumen ---
float calcularVolumen(float distancia) {
  if (distancia <= tabla[0][0]) return tabla[0][1];
  if (distancia >= tabla[numPuntos-1][0]) return tabla[numPuntos-1][1];

  for (int i = 0; i < numPuntos - 1; i++) {
    float x1 = tabla[i][0], y1 = tabla[i][1];
    float x2 = tabla[i+1][0], y2 = tabla[i+1][1];
    if (distancia >= x1 && distancia <= x2) {
      return y1 + (distancia - x1) * (y2 - y1) / (x2 - x1);
    }
  }
  return tabla[numPuntos-1][1]; // fallback seguro
}

// --- Mostrar datos en LCD ---
void mostrarLCD(float volumen, float porcentaje, float promedio) {
  lcd.setCursor(0,0);
  lcd.print(String(volumen, 0) + " ml      "); // limpiar resto de línea
  lcd.setCursor(0,1);
  if (porcentaje > 95.0) lcd.print("ALERTA: Alto    ");
  else if (porcentaje < 10.0) lcd.print("ALERTA: Critico");
  else lcd.print("Nivel: " + String(porcentaje,1) + "%   ");
}

// --- Mostrar datos en Serial ---
void mostrarSerial(float volumen, float porcentaje, float promedio) {
  Serial.print("Distancia media: "); Serial.print(promedio,2); Serial.println(" cm");
  Serial.print("Volumen estimado: "); Serial.print(volumen,1); Serial.println(" ml");
  Serial.print("Porcentaje: "); Serial.print(porcentaje,1); Serial.println(" %");
  if (porcentaje < 10.0) Serial.println("ALERTA: Nivel critico");
  else if (porcentaje < 25.0) Serial.println("ALERTA: Nivel bajo");
  else if (porcentaje > 95.0) Serial.println("ALERTA: Nivel alto");
  Serial.println("-----------------------------");
}
