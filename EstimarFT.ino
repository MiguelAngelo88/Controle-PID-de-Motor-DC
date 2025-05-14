const  int PWM_SAIDA = 27;
float RPM_eixo;

const  int pinA = 26;
volatile int contador = 0;
unsigned long tempoPassado = 0;
long intervalo = 100;

volatile long pulse_count = 0;
const int intervalo_ms = 100;  // Intervalo de amostragem
int pwm_value = 128;    // valor de 0 a 255

void setup() {
  pinMode(pinA, INPUT);
  pinMode(PWM_SAIDA, OUTPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(pinA), interrupcao, RISING);

  analogWrite(PWM_SAIDA, pwm_value);
}

void loop() {

  long count_ini = pulse_count;
  delay(intervalo_ms);
  long count_fim = pulse_count;

  long delta = count_fim - count_ini;
  
  RPM_eixo = 10 * contador * (60.0 / 374.0);

  unsigned long tempo = millis();

  Serial.print(tempo);
  Serial.print(",");
  Serial.print(pwm_value);
  Serial.print(",");
  Serial.println(RPM_eixo);
}

void interrupcao() {
  contador++;
}