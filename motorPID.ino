//https://youtu.be/bl2k6eXDAGM

int potPin = 25;
int PWM_SAIDA = 27;
float RPM_eixo;
float RPM_filtrado = 0;

float setPoint;
float setPoint_filtrado = 0;

int pinA = 26;
volatile int contador = 0;
unsigned long tempoPassado = 0;
long intervalo = 100;

float cv;
float cv1;
float erro;
float erro1;
float erro2;
float SinalDeControle;

float kp = 5;
float ki = 4;
float kd = 0.01;
float Tm = 0.1;

// Parâmetro do filtro
float alpha = 0.1; // quanto menor, mais suave

void setup() {
  pinMode(pinA, INPUT);
  pinMode(PWM_SAIDA, OUTPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(pinA), interrupcao, RISING);
}

void loop() {
  unsigned long tempoAtual = millis();

  if ((tempoAtual - tempoPassado) >= intervalo) {
    tempoPassado = tempoAtual;
    RPM_eixo = 10 * contador * (60.0 / 374.0);
    contador = 0;

    // Aplica filtro exponencial ao RPM
    RPM_filtrado = alpha * RPM_eixo + (1 - alpha) * RPM_filtrado;
  }

  // Leitura e filtro do setpoint
  setPoint = analogRead(potPin) * (210.0 / 4095.0); // 0-210 rpm
  setPoint_filtrado = alpha * setPoint + (1 - alpha) * setPoint_filtrado;

  // Cálculo do erro com valores filtrados
  erro = setPoint_filtrado - RPM_filtrado;

  // PID discreto (equação de diferenças)
  cv = cv1 + (kp + kd / Tm) * erro + (-kp + ki * Tm - 2 * kd / Tm) * erro1 + (kd / Tm) * erro2;
  cv1 = cv;
  erro2 = erro1;
  erro1 = erro;

  // Saturação da saída
  if (cv > 500.0) {
    cv = 500.0;
  }
  if (cv < 30.0) {
    cv = 30.0;
  }

  SinalDeControle = cv * (255.0 / 500.0);

  analogWrite(PWM_SAIDA, SinalDeControle); // 0-255 PWM proporcional

  // Plotar valores filtrados no Serial Plotter
  Serial.print(setPoint_filtrado);
  Serial.print(",");
  Serial.print(RPM_filtrado);
  Serial.print(",");
  Serial.print(SinalDeControle);
  Serial.print(",");

  //valores para manter a escala fixa entre 0 e 250 rpm
  Serial.print(0);
  Serial.print(",");
  Serial.println(250); // Valor fixo para forçar escala

  delay(100);
}

void interrupcao() {
  contador++;
}
