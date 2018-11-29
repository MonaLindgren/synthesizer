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
int btn2_flag,btn3_flag,btn4_flag,btn1_flag,nobtn_flag;
btn2_flag=0;
btn3_flag=0;
btn4_flag=0;
btn1_flag=0;
nobtn_flag=0;

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
  int sw = getsw();
  if (button==0 && nobtn_flag==0){  // no button used, set duty = 0, volume 0., only repeat if flag false
    genpwm(0,1);
    nobtn_flag=1;

    btn2_flag=btn3_flag=btn4_flag=0;
  }
  else {
    nobtn_flag = 0;
    if((button & 1) && (btn2_flag==0)){  //BTN2 pushed
      btn2_flag=1;
      genpwm(256,40496);
    }
    if((button>>1 & 1) && (btn3_flag==0)){ //BTN3 pushed
      btn3_flag=1;
      genpwm(256,42904);
    }
    if((button>>2 & 1) && (btn4_flag==0)){ //BTN4 pushed
      btn4_flag=1;
      genpwm(256,45454);
    }
  }



}

/* This function is called repetitively from the main program */
void labwork( void )
{
  checkfreq();
  prime = nextprime(prime);
  display_string(0,itoaconv(prime));
  display_update();
}
