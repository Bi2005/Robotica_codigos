// Pin del LED
const int ledPin = 13;
void setup() {
 pinMode(ledPin, OUTPUT);
}
void loop() {
 digitalWrite(ledPin, HIGH); // encender LED
 delay(500); // esperar 0.5 s
 digitalWrite(ledPin, LOW); // apagar LED
 delay(500); // esperar 0.5 s
}