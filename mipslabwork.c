/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount =0;
int prime = 1234567;
volatile int* trise = (volatile int*) 0xBF886100; //skapa pointers
volatile int* porte= (volatile int*) 0xBF886110;
volatile int* trisd= (volatile int*) 0xBF8860C0;
int btn2_flag=0;
int btn3_flag=0;
int btn4_flag=0;
int btn1_flag=0;
int nobtn_flag=0;
#define C5 38223
#define D5 34052
#define E5 30338
#define F5 28634
#define G5 25511
#define A5 22727
#define B5 20248
#define C6 19111

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  //
  // IFS(0)&= ~0x100;
  // timeoutcount++;
  // if(timeoutcount==10){
  // time2string(textstring,mytime);
  // display_string(3,textstring);
  // display_update();
  // tick(&mytime);
  // timeoutcount=0;}
  // return;
}
void genpwm(int dutycycle, int freq) {

  PR2 = freq;  //frequency
  OC1RS = dutycycle; // volume
  OC1CON= 0x7;
  T2CON = 0x20; // set bit 6-4, prescales 1:4
  T2CONSET = 0x8000; // start timer
  OC1CONSET = 0xFF00; // bit 15:8 output compare control register ON.

}

/* Lab-specific initialization goes here */
void labinit( void )
{
  *trisd |= 0xFE0; //enables btn2-4 and switch 1-4
  TRISF |= 0x2; //enables btn1 as input.
  // (*trise >> 8) << 8;  // clear 8 lsb ()
  // *porte=0; // set 0;
  // //TRISE = 0xFF;
  // T2CON=0x70; // bit 6-4 prescale 1:32, 101. page 14-9pic32 FRM
  // T2CONSET=0x8000;
  // PR2=(80000000/256)/10;
  // TMR2=0;
  // IEC(0)|=0x100; //enables interrupts from timer2, p90 FRM
  // IPC(2)|=0x1F; // priority
  // enable_interrupt();
  //genpwm(20);

  return;
}

void checkfreq( void ){
  int button = getbtn();
  display_string(0,itoaconv(button));
  int sw = getsw();
  if (button==0 && nobtn_flag==0){  // no button used, set duty = 0, volume 0., only repeat if flag false
    genpwm(0,1);
    nobtn_flag=1;
    btn1_flag=btn2_flag=btn3_flag=btn4_flag=0;
  }
  else {
    nobtn_flag = 0;
    if((button & 1) && (btn1_flag==0)){  //BTN1 pushed
      btn1_flag=1;
      genpwm(256, C5);
    }
    if((button>>1 & 1) && (btn2_flag==0)){ //BTN2 pushed
      btn2_flag=1;
      genpwm(256, D5);
    }
    if((button>>2 & 1) && (btn3_flag==0)){ //BTN3 pushed
      btn3_flag=1;
      genpwm(256, E5);
    }
    if((button>>3 & 1) && (btn4_flag==0)){ //BTN4 pushed
      btn4_flag=1;
      genpwm(256, F5);
    }
  }

}
int pot(){
unsigned int speed;
AD1PCFG = ~(1 << 2); // portb 2 analog pin with pot
TRISBSET = (1 << 2);

AD1CHS = (0x2 << 16);

AD1CON1 = (0x4 << 8) | (0x7 << 5);

AD1CON2 = 0x0;
AD1CON3 |= (0x1 << 15);
AD1CON1 |= (0x1 << 15);
AD1CON1 |= (0x1 << 1);

while(!(AD1CON1 & (0x1 << 1)));
while(!(AD1CON1 & 0x1));
speed = ADC1BUF0;
return speed;

}

/* This function is called repetitively from the main program */
void labwork( void )
{
  unsigned int speed;
  checkfreq();
  speed = pot();
  display_string(1,itoaconv(speed));

  //prime = nextprime(prime);
  //display_string(0,itoaconv(prime));
  display_update();
}
