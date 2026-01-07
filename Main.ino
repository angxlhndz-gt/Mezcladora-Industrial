#include <SPI.h>
int Cantidad; // Especificar la cantidad de piezas que se quieren hacer
int TiempoLimpieza = 300000;
const int CS = 10;
float porcentaje;
float temp;
int MotorPin = 2;
int Bomba1Pin = 6;
int Bomba2Pin = 8;
int Bomba3Pin = 10;
int SwitchPin = 4;
float MasaCatalizador;
float VolumenCatalizador;
float MasaResina;
float DensidadCatalizador = 1.18;
float VolumenResina;
float DensidadResina = 1.20;
float TiempoBombeoResina;
float TiempoBombeoCatalizador;
float TiempoBombeoThinner;
int TiempoMezclado;

void setup() {
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  pinMode(SwitchPin, INPUT);
  pinMode(MotorPin, OUTPUT);
  pinMode(Bomba1Pin, OUTPUT);
  pinMode(Bomba2Pin, OUTPUT);
  pinMode(Bomba3Pin, OUTPUT);
  digitalWrite(Bomba1Pin, LOW);
  digitalWrite(Bomba2Pin, LOW);
  digitalWrite(Bomba3Pin, LOW);
  digitalWrite(MotorPin, LOW);
}

float LeerTemp() {
  digitalWrite(CS, LOW);
  delayMicroseconds(10);

  uint8_t high = SPI.transfer(0);
  uint8_t low  = SPI.transfer(0);

  digitalWrite(CS, HIGH);

  int raw = ((high << 8) | low) >> 3;
  return raw * 0.25;
}

void CantidadDePiezas (){
  VolumenResina = Cantidad * 756;
  MasaResina = DensidadResina * VolumenResina;
}

void CalcularVolumenCatalizador (){
  MasaCatalizador = porcentaje * MasaResina;
  VolumenCatalizador = (MasaCatalizador) / (DensidadCatalizador);
}


void TiempoBombaResina (){
  TiempoBombeoResina = 15.6 * VolumenResina;
}


void TiempoBombaCatalizador (){
  TiempoBombeoCatalizador = 928.8 * VolumenCatalizador;
}


void TiempoBombaThinner (){
  TiempoBombeoThinner = TiempoBombeoResina * 1.15;
}

void PorcentajeTemp (){
  temp = LeerTemp();
  if (temp < 20) {
    porcentaje = 1.90;
    TiempoMezclado = 150000;
  }
  else if (temp >= 20 && temp < 24) {
      porcentaje = 1.80;
      TiempoMezclado = 130000;
  }
  else if (temp >= 24 && temp < 27) {
      porcentaje = 1.70;
      TiempoMezclado = 120000;
  }
  else if (temp >= 27 && temp < 30) {
      porcentaje = 1.65;
      TiempoMezclado = 100000;
  }
  else if (temp >= 30 && temp < 35) {
      porcentaje = 1.55;
      TiempoMezclado = 80000;
  }
  else {
      porcentaje = 0; // fuera de rango
  }
}
void loop() {
  if (digitalRead(SwitchPin) == HIGH) {   //Cuando el switch se enciende comienza el ciclo
    CantidadDePiezas();
    PorcentajeTemp();  
    CalcularVolumenCatalizador(); 
    TiempoBombaResina(); 
    TiempoBombaCatalizador(); 
    TiempoBombaThinner(); 
    digitalWrite(Bomba1Pin, HIGH); 
    delay(TiempoBombeoResina); 
    digitalWrite(Bomba1Pin, LOW); 
    digitalWrite(Bomba2Pin, HIGH); 
    delay(TiempoBombeoCatalizador); 
    digitalWrite(Bomba2Pin, LOW); 
    digitalWrite(MotorPin, HIGH); 
    delay(TiempoMezclado);
    digitalWrite(MotorPin, LOW); 

    delay(300000); // 5 Minutos para vertir la mezcla, luego comienza la limpieza
    
    digitalWrite(Bomba3Pin, HIGH); 
    delay(TiempoBombeoThinner); 
    digitalWrite(Bomba3Pin, LOW);  
  digitalWrite(MotorPin, HIGH); 
    delay(TiempoLimpieza); 
    digitalWrite(MotorPin, LOW); 
    
    //Se acaba el ciclo, se queda en modo reposo
  }

  
  

}
