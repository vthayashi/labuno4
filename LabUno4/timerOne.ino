#include "TimerOne.h"

uint32_t f8_cnt=0;

void tm1_setup(){
  pinMode(8,OUTPUT);
  Timer1.initialize(500); // Inicializa o Timer1 e configura para um período de 0,5 segundos
  Timer1.attachInterrupt(callback); // Configura a função callback() como a função para ser chamada a cada interrupção do Timer1
  t8=1000/f8;
}

void callback(){
  if(f8_cnt>=t8){
    digitalWrite(8, digitalRead(8) ^ 1);
    f8_cnt=1;
  }else{
    f8_cnt++;
  }
}
