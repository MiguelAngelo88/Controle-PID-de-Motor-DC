/*Ensaio em malha aberta (acionamento do motor + medição de velocidade)*/

const int PWM_SAIDA = 27; //Pin for PWM output
float RPM_eixo; //global variable for RPM

const int pinA = 26;           // Encoder pin
volatile long pulse_count = 0; // Keep volatile for ISR
const int intervalo_ms = 100;  // Sampling interval
int pwm_value = 200;           // PWM value (0-255)

void IRAM_ATTR interrupcao() {
  pulse_count++; // Simply increment the counter
}

void setup() {
  pinMode(pinA, INPUT);
  pinMode(PWM_SAIDA, OUTPUT);
  Serial.begin(115200);
  // Attach interrupt with IRAM_ATTR for ESP32
  attachInterrupt(digitalPinToInterrupt(pinA), interrupcao, RISING);
}

void loop() {
  analogWrite(PWM_SAIDA, pwm_value);

  // Calculate RPM and print every intervalo_ms
  static unsigned long lastTime = 0;
  unsigned long tempo = millis();

  if (tempo - lastTime >= intervalo_ms) {
    // Disable interrupts briefly to read pulse_count safely
    noInterrupts();
    long pulses = pulse_count;
    pulse_count = 0; // Reset counter for next interval
    interrupts();

    // Calculate RPM
    RPM_eixo = (pulses * 60.0 / 374.0) * (1000.0 / intervalo_ms);

    // Print in the format:pwm_value,RPM_eixo
    Serial.print(pwm_value);
    Serial.print(",");
    Serial.println(RPM_eixo);

    lastTime = tempo;
  }
}
