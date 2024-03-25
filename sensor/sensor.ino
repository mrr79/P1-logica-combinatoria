#define Pecho 6
#define Ptrig 7
long duracion, distancia;
int section; // Variable para almacenar la sección en código Gray

// Función para convertir decimal a código Gray
int decimalToGray(int num) {
  return num ^ (num >> 1);
}

// Función para imprimir un número en binario de 3 bits
void printBinary(int num) {
  for (int i = 2; i >= 0; i--) {
    Serial.print((num >> i) & 1);
  }
}

void setup() {
  Serial.begin (9600);       // Inicializa el puerto serie a 9600 baudios
  pinMode(Pecho, INPUT);     // Define el pin 6 como entrada (echo)
  pinMode(Ptrig, OUTPUT);    // Define el pin 7 como salida  (trig)
  pinMode(13, OUTPUT);       // Define el pin 13 como salida (para la alarma)
}

void loop() {
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // Genera el pulso de trig por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);

  duracion = pulseIn(Pecho, HIGH);
  distancia = (duracion / 2) / 29; // Calcula la distancia en centímetros
  
  // Limitar la distancia a 80 cm
  if (distancia > 80) {
    distancia = 80;
  }

  Serial.print("Distancia: ");
  Serial.print(distancia); // Envía el valor de la distancia por el puerto serie
  
  // Dividir el rango en 8 secciones
  section = map(distancia, 0, 80, 0, 7);
  
  // Ajustar la sección si está fuera de los límites
  if (section > 7) {
    section = 7;
  }
  
  // Convertir la sección a código Gray
  int graySection = decimalToGray(section);
  
  Serial.print(" cm, Sección decimal: ");
  Serial.print(section);
  Serial.print(" (binario: ");
  printBinary(section);
  Serial.print("), Sección en código Gray: ");
  Serial.print(graySection);
  Serial.print(" (binario: ");
  printBinary(graySection);
  Serial.println(")"); // Imprime la sección correspondiente en código Gray
  digitalWrite(13, LOW); // Pone el pin 13 en bajo

  if (distancia <= 50 && distancia >= 40) {
    digitalWrite(13, HIGH); // Pone el pin 13 en alto si la distancia es menor a 10cm
    Serial.println("Alarma seccion 4......."); // Envía la palabra Alarma por el puerto serie
  }

  if (distancia <= 70 && distancia >= 60) {
    digitalWrite(13, HIGH); // Pone el pin 13 en alto si la distancia es menor a 10cm
    Serial.println("Alarma seccion 6......."); // Envía la palabra Alarma por el puerto serie
  }
  delay(400); // Espera 400ms para que se logre ver la distancia en la consola
}
