// Condições de funcionamento
int TimerDefault  = 18000;  // Tempo limite em segundos para desativar o relay caso o sensor não se desligue.
int Debug         = 0;      // 0 = funcionamento normal; 1 = Modo Debug, envia mensagens para o monitor série indicando a operação que está a executar

// Pin attributions
int pinSensor     = 12;     // Sensor liga ao pino 12
int pinRelay      = 2;      // Relay liga ao pino 10
int pinAlarmLED   = 13;     // Led Alarme liga ao pino 13
int pinAlarmReset = 8;      // Botão para fazer reset ao alarme liga ao pino 8

// States
int TimerCount;             // Contador do temporizador. Este contador inicia-se no valor do TimerDefault e vai reduzindo segundo a segundo até 0. Quando chega a 0, desliga o relay e ativa o alarme
int RelayState    = 0;      // Estado atual do relay
int AlarmState    = 0;      // Estado atual do alarme. Enquanto for 0, o relay pode ser ligado ou desligado. Assim que passa a 1, o relay é desligado

void setup() {
  if (Debug == 1) {         // Ativa o monitor série caso esteja em modo Debug
    Serial.begin(9600);
  }

  TimerDefault = TimerDefault * 4;

  // Prepara os modos respetivos dos pinos a utilizar
  pinMode(pinSensor, INPUT_PULLUP);
  pinMode(pinRelay, OUTPUT);
  pinMode(pinAlarmLED, OUTPUT);
  pinMode(pinAlarmReset, INPUT_PULLUP);
  Log("Preparados os modos respetivos dos pinos a utilizar");

  RelayOff();
}

void loop() {    
  int Button = digitalRead(pinAlarmReset);  // Lê o botão de reset
  Log("Lido o botão de reset");  

  if (Button == LOW) {
    ResetAlarm();
  }
  else {
    int Sensor = digitalRead(pinSensor);    // Lê o sensor
    Log("Lido o estado do sensor");

    // Se o sensor está ativo e o tempo limite não tiver sido atingido, liga o relay (caso não esteja já ligado), e decrementa o contador. Se não estiver, faz o reset.
    if (Sensor == LOW) {
      if (TimerCount > 0) {
        RelayOn();
      }
      else {
        SetAlarm();
      }
      TimerCount = TimerCount - 1; // Decrementa um segundo ao contador
      Log("Decrementado o contador");
    }
    else {
      RelayOff();     
    }
  }

  delay(250);
}

// Reset para as condições iniciais
void RelayOff() {
  digitalWrite(pinRelay, LOW);  
  RelayState = 0;  
  TimerCount = TimerDefault;
  Log("Desligado o relay e repostas as condições iniciais");
}

// Liga o relay caso não esteja o alarme ligado
void RelayOn() {
  if ((RelayState == 0) && (AlarmState == 0)) {
    digitalWrite(pinRelay, HIGH);    
  }  
  RelayState = 1;
  Log("Ligado o relay");
}

// Liga o alarme
void SetAlarm() {
  digitalWrite(pinAlarmLED, HIGH);
  AlarmState = 1;
  Log("Ativado o alarme");
  RelayOff();  
}

// Desliga o alarme
void ResetAlarm() {
  digitalWrite(pinAlarmLED, LOW);
  AlarmState = 0;
  Log("Desativado o alarme");
  RelayOff();  
}

// Envia uma mensagem para o monitor série, caso esteja ativo o modo Debug
void Log(char Text[]) {
  if (Debug == 1) {
    Serial.println(Text);
  }
}