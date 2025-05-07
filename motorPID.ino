//https://youtu.be/bl2k6eXDAGM

int potPin = 25;
int PWM_SAIDA = 26;
float RPM_eixo;
float setPoint;

int pinA = 27;
volatile int contador = 0;
unsigned long tempoPassado = 0;
long intervalo = 100;

float cv;
float cv1;
float erro;
float erro1;
float erro2;

float kp = 1;
float ki = 1;
float kd = 0.01;
float Tm = 0.1;

void setup() {
  pinMode(pinA, INPUT);
  pinMode(PWM_SAIDA, OUTPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(pinA), interrupcao, RISING);

}

void loop() {
  unsigned long tempoAtual = millis();

  if((tempoAtual - tempoPassado) >= intervalo){
    tempoPassado = tempoAtual;
    RPM_eixo = 10*contador*(60.0/374.0);
    contador = 0;
  }

  //set point
  setPoint = analogRead(potPin)*(210.0/1023.0); //0-210 rpm

  //Erro
  erro = setPoint - RPM_eixo;

  //Equação de diferenças
  cv = cv1 + (kp + kd/Tm)*erro + (-kp+ki*Tm-2*kd/Tm)*erro1 + (kd/Tm)*erro2;
  cv1 = cv;
  erro2 = erro1;
  erro1 = erro;

  //Saturando a saida do PID
  if( cv > 500.0){
    cv = 500.0;
  }
  if(cv < 30.0){
    cv = 30.0;
  }


  analogWrite(PWM_SAIDA, cv*(255.0/500.0)); //0-255

  Serial.print("SETPOINT: ");
  Serial.print(setPoint);
  Serial.print(", RPM DO EIXO: ");
  Serial.print(RPM_eixo);

  delay(100);
}

void interrupcao(){
  contador++;
}
