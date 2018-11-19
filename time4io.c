#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

volatile int* portd= (volatile int*) 0xBF8860D0;

int getsw (void){
  return (*portd>> 8) & 0xF;
}

int getbtn (void){

  return (*portd >> 5) & 0x7;
}
