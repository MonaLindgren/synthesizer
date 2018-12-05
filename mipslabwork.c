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
#include <string.h>

int mytime = 0x5957;
int timeoutcount =0;
int timeoutcount2 =0;
int prime = 1234567;
volatile int* trise = (volatile int*) 0xBF886100; //skapa pointers
volatile int* porte= (volatile int*) 0xBF886110;
volatile int* trisd= (volatile int*) 0xBF8860C0;
int testsong[] = {40496,30000,40496,30000};
int songsize = sizeof(testsong)/sizeof(testsong[0]);
int notecount =0;
int k=0;
int playnote=0;


int btn2_flag=0;
int btn3_flag=0;
int btn4_flag=0;
int btn1_flag=0;
int nobtn_flag=0;
int prev_sw = 0; // store the previous input from switch 1.

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
void initt4(void){
  T4CON= 0x70;
  T4CONSET = 0x8000;
  PR4 = (80000000/256)/10;
  TMR4=0;

}

/*
* checkfreq takes input from the buttons and switch 1 
* and plays the note that correspond to that specific button-switch setting. 
* checkfreq checks the last switch1-value and look up the current switch1-value.
* This is required so that the buttons can be reset properly.
*/
void checkfreq( int dutycycle ){
  int button = getbtn();
  //display_string(0,itoaconv(button)); // TESTING
  int sw = getsw();
  int sw1_flag = sw>>3 & 1; // check if switch 1 is on.
  if(sw1_flag != prev_sw){ 
  	display_string(0,"reset buttons");
  	prev_sw = sw1_flag;
  	btn1_flag = 0; //reset all pressed-button-flags
  	btn2_flag = 0;
  	btn3_flag = 0;
  	btn4_flag = 0;	
  }
  if (button==0 && nobtn_flag==0){  // no button used, set duty = 0, volume 0., only repeat if flag false
    genpwm(0,1);
    nobtn_flag=1;
    btn1_flag=btn2_flag=btn3_flag=btn4_flag=0;
  }
  else {
    nobtn_flag = 0;
    if((button & 1) && (btn1_flag==0) && !sw1_flag){  //BTN1 pushed and no transpose
      btn1_flag=1;
      genpwm(dutycycle, F5);
    }
    else if((button & 1) && (btn1_flag==0) && sw1_flag){  //BTN1 pushed and switch-1-transpose
      btn1_flag=1;
      genpwm(dutycycle, C6);
    }
    else if((button>>1 & 1) && (btn2_flag==0) && !sw1_flag){ //BTN2 pushed and no transpose
      btn2_flag=1;
      genpwm(dutycycle, E5);
    }
    else if((button>>1 & 1) && (btn2_flag==0) && sw1_flag){ //BTN2 pushed and switch-1-transpose
      btn2_flag=1;
      genpwm(dutycycle, B5);
    }
    else if((button>>2 & 1) && (btn3_flag==0) && !sw1_flag){ //BTN3 pushed and no transpose
      btn3_flag=1;
      genpwm(dutycycle, D5);
    }
    else if((button>>2 & 1) && (btn3_flag==0) && sw1_flag){ //BTN3 pushed and switch-1-transpose
      btn3_flag=1;
      genpwm(dutycycle, A5);
    }
    else if((button>>3 & 1) && (btn4_flag==0) && !sw1_flag){ //BTN4 pushed and no transpose
      btn4_flag=1;
      genpwm(dutycycle, C5);
    }
    else if((button>>3 & 1) && (btn4_flag==0) && sw1_flag){ //BTN4 pushed and switch-1-transpose
      btn4_flag=1;
      genpwm(dutycycle, G5);
    }
  }
}

// function that plays notes from at the moment testsong.
// TODO delay /playtime not yet configured

void playsong(int dutycycle, int ms){
    if(IFS(0)&0x100){
      IFS(0)&= ~0x100;
      timeoutcount++;
      display_string(0,itoaconv(timeoutcount));
      if(timeoutcount==ms/2 && playnote==0){
          genpwm(dutycycle,testsong[k]);
          playnote=1;
        }
      if(timeoutcount==ms && playnote==1){
        genpwm(0,1);
        timeoutcount=0;
        notecount++;
        k++;
        playnote=0;
      }
      if(notecount==songsize){
        k=0;
        notecount=0;

      }
    }
  }
// function that returns a value between 0-1023 controlled by potentiometer
// TODO Comment code move to initialization
int pot( void ){
unsigned int value;
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
value = ADC1BUF0;
return value;
}

// If switch is on do this -- TODO  create delay that works

/* This function is called repetitively from the main program */
void labwork( void )
{
  unsigned int volume,volumepercent;
  int sw = getsw();
  int sw2 = sw>>2;
  //char volstring[80]= "Volume: ";

  //char block[] = "á";
  volume = pot();
  volumepercent = volume/10.23;
  //strcat(volstring,itoaconv(volumepercent));


  display_string(1,itoaconv(volumepercent));
  //display_string(2,block);
  //display_image(96,testicon);
  if(sw2 & 1){
    initt4();
    playsong(volume,1000);
  }
  else{
  checkfreq(volume);}

  //prime = nextprime(prime);
  //display_string(0,itoaconv(prime));
  display_update();
}
