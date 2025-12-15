const int ledPin = 13;
const int buttonPin = 2;

void setup() {
 pinMode(ledPin, OUTPUT);
 pinMode(buttonPin, INPUT_PULLUP); // utiliza resistencia pull-up interna
}

void loop() {
 int boton = digitalRead(buttonPin);
 if (boton == LOW) { // botón presionado
 digitalWrite(ledPin, HIGH);
 } else {
 digitalWrite(ledPin, LOW);
 }
}
