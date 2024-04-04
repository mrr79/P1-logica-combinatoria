//#include <LiquidCrystal.h>

//Lecturas de las señales retroalimentadas 
//const int binFB0 = A5;
//const int binFB1 = A4;
//const int binFB2 = A3;

// select the pins used on the LCD panel
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int numBin[2]; // Arreglo para almacenar la representación binaria del número
int numGrey[2];      // Arreglo para almacenar la representación de código Gray
int comparacion;  // Variable para realizar la suma de bits binarios

const int Trigger = 2; // Pin digital 2 para el Trigger del sensor
const int Echo = 3;    // Pin digital 3 para el Echo del sensor


void setup(void) {
  // Inicialización de comunicación serial
  Serial.begin(9600);

  //Pines de entrada de la retroalimentación
  //pinMode(binFB0, INPUT);
  //pinMode(binFB1, INPUT);
  //pinMode(binFB2, INPUT);

  // Configuración del sensor de ultrasonido
  pinMode(Trigger, OUTPUT); // Pin como salida para el Trigger
  pinMode(Echo, INPUT);     // Pin como entrada para el Echo

  // Configuración de pines como salidas digitales
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

/*void LCD(int numero, int entrada){
  if (entrada >= 8){
    lcd.begin(16, 2);              // start the library
    lcd.setCursor(0,0);
    lcd.print("Hexadecimal"); 
    lcd.setCursor(14,0);
    lcd.print(0);              
    lcd.setCursor(0,1);
    lcd.print("Gray "); 
    lcd.setCursor(14,1);
    lcd.print(0);
  }else{
    lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("Hexadecimal"); 
  lcd.setCursor(14,0);
  lcd.print(numero); 
  lcd.setCursor(0,1);
  lcd.print("Gray "); 
  lcd.setCursor(14,1);
  lcd.print(entrada);
  }
}*/

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
void Grey() {
  numGrey[0] = numBin[0];
  numGrey[1] = sumaBinario(numBin[0], numBin[1]);
  numGrey[2] = sumaBinario(numBin[1], numBin[2]);
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
  //Leer el valor de la retroalimentación}
  //int bitFB2 = digitalRead(binFB2);
  //int bitFB1 = digitalRead(binFB1);
  //int bitFB0 = digitalRead(binFB0);

  //int hexadecimal = ((bitFB2 * 4) + (bitFB1 * 2) + (bitFB0 * 1));

  // Genera un pulso corto en el pin de Trigger del sensor de ultrasonido
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  // Lee el tiempo que tarda en llegar el eco
  long duracion = pulseIn(Echo, HIGH);

  // Calcula la distancia en centímetros
  int distancia = (duracion / 2) / 29; // Calcula la distancia en centímetros

  // Limitar la distancia a 80 cm
  if (distancia > 40) {
    distancia = 40;
  }

  // Impresión de la distancia por serial


  // Mapeo de la distancia a un rango entre 0 y 7
  Serial.print("Distancia: ");
  Serial.print(distancia); // Envía el valor de la distancia por el puerto serie

  // Convertir la distancia a la sección correspondiente
  int section = map(distancia, 0, 40, 0, 7);


  //Mostrar en LCD
  //LCD(hexadecimal, redondeo);
  // Conversión del número redondeado a su representación binaria
  Binario(section);

  // Impresión del número redondeado por serial
  Serial.println(section);
  Serial.println("binario");
  Serial.println(numBin[0]); // Impresión del primer bit binario
  Serial.println(numBin[1]); // Impresión del segundo bit binario
  Serial.println(numBin[2]); // Impresión del tercer bit binario

  // Conversión de la representación binaria a código Gray
  Grey();
  Serial.println("grey");
  Serial.println(numGrey[0]); // Impresión del A de código Gray
  Serial.println(numGrey[1]); // Impresión del B de código Gray
  Serial.println(numGrey[2]); // Impresión del C de código Gray 

  // Escritura en los pines digitales según la representación de código Gray
  escribirCompuerta();

  // Retardo de 1 segundo antes de repetir el bucle
  delay(1000);
}