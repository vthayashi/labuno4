#define Versao "LabUno4"

uint32_t f8=100; //max=1000;
uint32_t t8;

void setup() {
  tm1_setup();
  Serial.begin(500000);
}

void loop() {
  serial_loop();
}
