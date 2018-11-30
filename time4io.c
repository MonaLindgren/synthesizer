#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

volatile int* portd= (volatile int*) 0xBF8860D0;

int getsw (void){
  return (*portd>> 8) & 0xF;
}

/*
* getbtn returns a 4-bit number containing the input signal
* from btn 1 - 4
* bit 0 (lsb) : btn1
* bit 1 : btn2
* bit 2 : btn3
* bit 3 : btn4
*/
int getbtn (void){
	int tmp = (*portd >> 5) & 0x7;
	tmp <<= 1;
	tmp |= (PORTD >> 1) & 1;
	return tmp;
}
