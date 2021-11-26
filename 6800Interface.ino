#define CLK_BUTTON_PIN  13
#define CLK_SWITCH_PIN  12
#define CLK_P1_PIN      11
#define CLK_P2_PIN      10
#define CLK_POT_PIN     A0

#define VMA_PIN 9
#define RW_PIN  8

const uint8_t ADDR[] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 28, 26, 24, 22};
const uint8_t DATA[] = {52, 50, 48, 46, 44, 42, 40, 38};

unsigned long lastClkEdge   = 0;
         bool clkButtonIsOn = false;

void setup() {
  pinMode(CLK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CLK_SWITCH_PIN, INPUT_PULLUP);
  pinMode(CLK_P1_PIN,     OUTPUT);
  pinMode(CLK_P2_PIN,     OUTPUT);
  
  pinMode(VMA_PIN,        INPUT);
  pinMode(RW_PIN,         INPUT);

  for (int i = 0; i < 16; i++)
    pinMode(ADDR[i], INPUT);

  for (int i = 0; i < 8; i++)
    pinMode(DATA[i],    INPUT);

  Serial.begin(115200);
}

void loop() {
  if (digitalRead(CLK_SWITCH_PIN)) {
    if (digitalRead(CLK_BUTTON_PIN) == LOW && clkButtonIsOn == false && lastClkEdge < millis() - 30) {
      lastClkEdge = millis();
      clkButtonIsOn = true;
      pulseClk();
      printData();
    }
    else if (digitalRead(CLK_BUTTON_PIN) == HIGH && clkButtonIsOn == true && lastClkEdge < millis() - 30) {
      lastClkEdge = millis();
      clkButtonIsOn = false;
    }
  }
  else {
    digitalWrite(CLK_P1_PIN, LOW);
    digitalWrite(CLK_P2_PIN, LOW);
  }

//  if (digitalRead(VMA_PIN) == HIGH) {
//    for (int i = 0; i < 16; i++) {
//      Serial.print(digitalRead(ADDR[i]));
//    }
//    Serial.println();
//  }
}

void pulseClk() {
  digitalWrite(CLK_P1_PIN, HIGH);
  digitalWrite(CLK_P2_PIN, LOW);
  delay(100);
  digitalWrite(CLK_P1_PIN, LOW);
  digitalWrite(CLK_P2_PIN, HIGH);
}

void printData() {
  char output[15];

  unsigned int address = 0;
  for (int i = 0; i < 16; i++) {
    int readBit = digitalRead(ADDR[i]) ? 1 : 0;
    Serial.print(readBit);
    address = (address << 1) + readBit;
  }

  Serial.print(' ');  

  unsigned int data = 0;
  for (int i = 0; i < 8; i++) {
    int readBit = digitalRead(DATA[i]) ? 1 : 0;
    Serial.print(readBit);
    data = (data << 1) + readBit;
  }

  sprintf(output, "   %04x  %c %02x", address, digitalRead(RW_PIN) ? 'r' : 'W', data);
  Serial.println(output);
}
