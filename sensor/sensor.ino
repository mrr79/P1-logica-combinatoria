// Declaración de arreglos y variables globales
int numBin[2]; // Arreglo para almacenar la representación binaria del número
int numGrey[2];      // Arreglo para almacenar la representación de código Gray
int comparacion;  // Variable para realizar la suma de bits binarios

const int Trigger = 2; // Pin digital 2 para el Trigger del sensor
const int Echo = 3;    // Pin digital 3 para el Echo del sensor


void setup(void) {
  // Inicialización de comunicación serial
  Serial.begin(9600);

   // Configuración del sensor de ultrasonido
  pinMode(Trigger, OUTPUT); // Pin como salida para el Trigger
  pinMode(Echo, INPUT);     // Pin como entrada para el Echo

  // Configuración de pines como salidas digitales
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

// Función para convertir un número a su representación binaria
void Binario(int numero) {
  switch (numero) {
    // Casos para los números del 1 al 7
    case 1:
      numBin[0] = 0;
      numBin[1] = 0;
      numBin[2] = 1;
      break;
    case 2:
      numBin[0] = 0;
      numBin[1] = 1;
      numBin[2] = 0;
      break;
    case 3:
      numBin[0] = 0;
      numBin[1] = 1;
      numBin[2] = 1;
      break;
    case 4:
      numBin[0] = 1;
      numBin[1] = 0;
      numBin[2] = 0;
      break;
    case 5:
      numBin[0] = 1;
      numBin[1] = 0;
      numBin[2] = 1;
      break;
    case 6:
      numBin[0] = 1;
      numBin[1] = 1;
      numBin[2] = 0;
      break;
    case 7:
      numBin[0] = 1;
      numBin[1] = 1;
      numBin[2] = 1;
      break;
    // Caso por defecto para otros valores (0 o fuera de rango)
    default:
      numBin[0] = 0;
      numBin[1] = 0;
      numBin[2] = 0;
  }
}

// Función para escribir en los pines digitales según la representación de código Gray
void escribirCompuerta() {
  for (int i = 0; i < 3; i++) {
    if (numGrey[i] == 1) {
      digitalWrite(i + 10, HIGH); // Encender el pin correspondiente
    } else {
      digitalWrite(i + 10, LOW);  // Apagar el pin correspondiente
    }
  }
}

// Función para convertir la representación binaria a código Gray
// Función para convertir la representación binaria a código Gray
void Grey() {
  numGrey[0] = numBin[0];
  numGrey[1] = numBin[0] ^ numBin[1];  // XOR entre el primer y segundo bit
  numGrey[2] = numBin[1] ^ numBin[2];  // XOR entre el segundo y tercer bit
}


// Función para sumar dos bits binarios y obtener el resultado
int sumaBinario(int num1, int num2) {
  comparacion = 0;             // Reinicia la variable de comparación
  comparacion = num1 + num2;   // Realiza la suma de los bits
  if (comparacion == 1) {
    return comparacion;        // Si la suma es 1, retorna 1
  } else {
    return 0;                  // Si no, retorna 0
  }
}

void loop() {
  // Genera un pulso corto en el pin de Trigger del sensor de ultrasonido
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);   // Genera el pulso de trig por 10ms
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  // Lee el tiempo que tarda en llegar el eco
  long duracion = pulseIn(Echo, HIGH);

  // Calcula la distancia en centímetros
  int distancia = (duracion / 2) / 29; // Calcula la distancia en centímetros

  // Limitar la distancia a 80 cm
  if (distancia > 80) {
    distancia = 80;
  }

  Serial.print("Distancia: ");
  Serial.print(distancia); // Envía el valor de la distancia por el puerto serie

  // Convertir la distancia a la sección correspondiente
  int section = map(distancia, 0, 80, 0, 7);

  // Impresión de la sección decimal y binaria por serial
  Serial.print(" Sección decimal: ");
  Serial.print(section);
  Serial.print(" (binario: ");
  Serial.print(section / 4);
  section %= 4;
  Serial.print((section / 2) % 2);
  Serial.print(section % 2);
  Serial.print(") ");

  // Conversión de la representación binaria a código Gray
  Grey();

  // Impresión de la sección en código Gray binario y su representación binaria por serial
  Serial.print("Sección en código Gray binario: ");
  Serial.print(numGrey[0]);
  Serial.print(numGrey[1]);
  Serial.print(numGrey[2]);
  Serial.print("  ______");
  Serial.println();

  // Escritura en los pines digitales según la representación de código Gray
  escribirCompuerta();

  // Retardo de 1 segundo antes de repetir el bucle
  delay(1000);
}