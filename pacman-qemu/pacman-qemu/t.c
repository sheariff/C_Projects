/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "defines.h"
#include "string.c"
struct sprite {
   int x,y;
   int buff[16][16];
   int replacePix;
   char *p;
   int oldstartRow;
   int oldstartCol;
};

int spriteMove = 0;
char *tab = "0123456789ABCDEF";
int color;

#include "timer.c"

#include "interrupts.c"
#include "kbd.c"
#include "uart.c"
#include "vid.c"
extern char _binary_panda_bmp_start;
extern char _binary_porkcar_bmp_start;
extern char _binary_pacman_bmp_start;
extern char _binary_speedy_bmp_start;
extern char _binary_pokey_bmp_start;
extern char _binary_power_bmp_start;
extern char _binary_mur_bmp_start;
extern char _binary_point_bmp_start;


void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}
int kprintf(char *fmt, ...);
void timer_handler();
void vid_handler();

void uart0_handler()
{
  uart_handler(&uart[0]);
}
void uart1_handler()
{
  uart_handler(&uart[1]);
}

// IRQ interrupts handler entry point
//void __attribute__((interrupt)) IRQ_handler()
// timer0 base=0x101E2000; timer1 base=0x101E2020
// timer3 base=0x101E3000; timer1 base=0x101E3020
// currentValueReg=0x04
TIMER *tp[4];

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC SIV status registers to find out which interrupt
    vicstatus = VIC_STATUS;
    sicstatus = SIC_STATUS;  
    //kprintf("vicstatus=%x sicstatus=%x\n", vicstatus, sicstatus);
    // VIC status BITs: timer0,1=4, uart0=13, uart1=14, SIC=31: KBD at 3
    /**************
    if (vicstatus & 0x0010){   // timer0,1=bit4
      if (*(tp[0]->base+TVALUE)==0) // timer 0
         timer_handler(0);
      if (*(tp[1]->base+TVALUE)==0)
         timer_handler(1);
    }
    if (vicstatus & 0x0020){   // timer2,3=bit5
       if(*(tp[2]->base+TVALUE)==0)
         timer_handler(2);
       if (*(tp[3]->base+TVALUE)==0)
         timer_handler(3);
    }
    if (vicstatus & 0x80000000){
       if (sicstatus & 0x08){
          kbd_handler();
       }
    }
    *********************/
    /******************
    if (vicstatus & (1<<4)){   // timer0,1=bit4
      if (*(tp[0]->base+TVALUE)==0) // timer 0
         timer_handler(0);
      if (*(tp[1]->base+TVALUE)==0)
         timer_handler(1);
    }
    if (vicstatus & (1<<5)){   // timer2,3=bit5
       if(*(tp[2]->base+TVALUE)==0)
         timer_handler(2);
       if (*(tp[3]->base+TVALUE)==0)
         timer_handler(3);
    }
    *********************/
    if (vicstatus & (1<<4)){   // timer0,1=bit4
       timer_handler(0);
    }   
    if (vicstatus & (1<<12)){   // bit 12: uart0 
         uart0_handler();
    }
    if (vicstatus & (1<<13)){   // bit 13: uart1
         uart1_handler();
    }
    if (vicstatus & (1<<16)){   // bit 16: video
         vid_handler();
        VIC_CLEAR &= ~(1<<16);    
    }
    if (vicstatus & (1<<31)){
      if (sicstatus & (1<<3)){
       //   kbd_handler();
       }
    }
VIC_CLEAR =0;
}
extern int oldstartR;
extern int oldstartC;
extern int replacePix;
extern int buff[16][16];
struct sprite sprites[3];
#define Width  19
#define Height 21
enum Objet {V, M, P, W};

//Tableau de la carte
enum Objet table[Height][Width] =
{
	{ M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M},
	{ M, P, P, P, P, P, P, P, P, M, P, P, P, P, P, P, P, P, M},
	{ M, W, M, M, P, M, M, M, P, M, P, M, M, M, P, M, M, W, M},
	{ M, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, M},
	{ M, P, M, M, P, M, P, M, M, M, M, M, P, M, P, M, M, P, M},
	{ M, P, P, P, P, M, P, P, P, M, P, P, P, M, P, P, P, P, M},
	{ M, M, M, M, P, M, M, M, V, M, V, M, M, M, P, M, M, M, M},
	{ V, V, V, M, P, M, V, V, V, V, V, V, V, M, P, M, V, V, V},
	{ M, M, M, M, P, M, V, M, M, V, M, M, V, M, P, M, M, M, M},
	{ V, V, V, V, P, V, V, M, V, V, V, M, V, V, P, V, V, V, V},
	{ M, M, M, M, P, M, V, M, M, M, M, M, V, M, P, M, M, M, M},
	{ V, V, V, M, P, M, V, V, V, V, V, V, V, M, P, M, V, V, V},
	{ M, M, M, M, P, M, V, M, M, M, M, M, V, M, P, M, M, M, M},
	{ M, P, P, P, P, P, P, P, P, M, P, P, P, P, P, P, P, P, M},
	{ M, P, M, M, P, M, M, M, P, M, P, M, M, M, P, M, M, P, M},
	{ M, W, P, M, P, P, P, P, P, P, P, P, P, P, P, M, P, W, M},
	{ M, M, P, M, P, M, P, M, M, M, M, M, P, M, P, M, P, M, M},
	{ M, P, P, P, P, M, P, P, P, M, P, P, P, M, P, P, P, P, M},
	{ M, P, M, M, M, M, M, M, P, M, P, M, M, M, M, M, M, P, M},
	{ M, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, M},
	{ M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M, M},
};
enum Objet map[Height*8][Width*8];
int mapx=0,mapy=0;

void box(int x,int y,int xsize,int ysize){
int i=0,j=0;
for (i=1;i<xsize;i++)
    kpchar(205,y,x+i);
for (j=1;j<ysize;j++){
    kpchar(186,y+j,x);
    kpchar(186,y+j,x+xsize);
}
for (i=1;i<xsize;i++)
    kpchar(205,y+ysize,x+i);
kpchar(201,y,x); // left top

kpchar(187,y,x+xsize); // right top
kpchar(188,y+ysize,x+xsize); // right bottom
kpchar(200,y+ysize,x); //left bottom
}
struct High{
char name[20];
int score;
};
struct High scores[20];
int noscores;
Draw_all(){
char *p;
int i=0,j=0;
for (int x=0; x<640*480; x++)
    fb[x] = 0x00000000;    // clean screen; all pixels are BLACK
 TIMER *t = &timer[0];
for (i=0; i<8; i++){
       kpchar(t->clock[i], 1, 70+i);
    }
box(50,2,20,15);
for (j=0;j<noscores;j++)
    for (i=0; scores[j].name[i]; i++){
       kpchar(scores[j].name[i], 4+j, 51+i);
    }
//p = &_binary_panda_bmp_start;
 //  show_bmp1(p, 0, 80);
for (int j=mapy;j<mapy+Height;j++)
    for (int i=mapx;i<mapx+Width;i++){
        if (table[j][i] == M){
           show_bmp1(&_binary_mur_bmp_start, (j-mapy)*16, (i-mapx)*16);
           }
	if (table[j][i] == P){
           show_bmp1(&_binary_point_bmp_start, (j-mapy)*16, (i-mapx)*16);
           }
}
     
for ( i=0;i<3;i++){
    show_bmp1(sprites[i].p, sprites[i].y, sprites[i].x);
    if (i)
         sprites[i].x+=10; 
    }
fbmodified =1;
*(volatile unsigned int *)(0x1012001C) |= 0xc; // enable video interrupts
}
int counter;


int main()
{
   int i,j; 
   char line[128], string[32]; 
   UART *up;
  scores[0].score = 3000;
  kstrcpy(scores[0].name,"joe");
  scores[1].score = 2998;
kstrcpy(scores[1].name,"mary");
scores[2].score = 2991;
kstrcpy(scores[2].name,"fred");
scores[3].score = 2222;
kstrcpy(scores[3].name,"jane");
noscores=4;
  int x=80;
   int y=0;
   sprites[0].x = 80;
   sprites[0].y = 0;
   sprites[0].replacePix =0;
   sprites[0].p = &_binary_pacman_bmp_start;
   sprites[1].x = 200;
   sprites[1].y = 200;
   sprites[1].replacePix =0;
   sprites[1].p = &_binary_pokey_bmp_start;
   sprites[2].x = 600;
   sprites[2].y = 400;
   sprites[2].replacePix =0;
   sprites[2].p = &_binary_speedy_bmp_start;

   char * p;   
   color = YELLOW;
   row = col = 0; 


   /* enable timer0,1, uart0,1 SIC interrupts */
  VIC_INTENABLE |= (1<<4);  // timer0,1 at bit4 
  // VIC_INTENABLE |= (1<<5);  // timer2,3 at bit5 

   VIC_INTENABLE |= (1<<12); // UART0 at 12
  // VIC_INTENABLE |= (1<<13); // UART1 at 13

   VIC_INTENABLE |= (1<<16); // LCD at 16
 
   UART0_IMSC = 1<<4;  // enable UART0 RXIM interrupt
   UART1_IMSC = 1<<4;  // enable UART1 RXIM interrupt

  //VIC_INTENABLE |= 1<<31;   // SIC to VIC's IRQ31

   /* enable KBD IRQ */
   SIC_ENSET = 1<<3;  // KBD int=3 on SIC
   SIC_PICENSET = 1<<3;  // KBD int=3 on SIC
fbuf_init();
   kprintf("C3.3 start: Interrupt-driven drivers for Timer KBD UART\n");
   timer_init();

   /***********
   for (i=0; i<4; i++){
      tp[i] = &timer[i];
      timer_start(i);
   }
   ************/
   timer_start(0);
   kbd_init();

   uart_init();
   up = &uart[0]; 
   box(50,2,20,8);
   row = 3;
   col =51;
   kprintf("   High Scores");
   clrcursor();
for (j=0;j<noscores;j++){
    //for (i=0; scores[j].name[i]; i++){
       row = 4+j;
       col =51;
       kprintf("%s %d",scores[j].name,scores[j].score);
       clrcursor();
       //for (i=0;i<20
       //kpchar(scores[j].name[i], 4+j, 51+i);
    }
  
   // p = &_binary_pacman_bmp_start;
   //show_bmp(p, 0, 80,buff,replacePix,&oldstartR,&oldstartC);
 
  // while(1);
   
   /*
   while(1){
      color = CYAN;
      kprintf("Enter a line from KBD\n");
      kgets(line);
   }
   */
   unlock();
 kprintf("\nenter KBD here\n");
uuprints("from UART0 here\n\r");
int move=0;
int key;
   while(1){
//uprintf("enable %x\n",*(volatile unsigned int *)(0x1012001C));
//uprintf("rand %d\n",(rand()+1)%4);
    if (vidhandler){
    // uprintf("got vidhandler %x counter %d\n",*(volatile unsigned int *)(0x1012001C),counter);
      vidhandler=0;
      counter++;
}
     move=0;
     replacePix =1;
     if (upeek(up)){
     key=ugetc(up);
     switch(key){
       case 's':
         if ( sprites[0].x )
            sprites[0].x-=16;
         move =1;
         break;
       case 'd':
         if ( sprites[0].x< 16*(Width-1) )
            sprites[0].x+=16;
          move =1;
         break;
      case 'w':
         if ( sprites[0].y )
            sprites[0].y-=16;
          move =1;
         break;
      case 'e':
         if ( sprites[0].y< 16*(Height-1) )
            sprites[0].y+=16;
         move=1;
         break;
      default:
        move=0;
      }
}
      if (move || spriteMove){
         // switch_buffer();
          Draw_all();
          spriteMove=0;
        }
      /*if (move)
         show_bmp(p, y, x,buff,replacePix,&oldstartR,&oldstartC);
      if (spriteMove){
          show_bmp(sprites[0].p, sprites[0].y, sprites[0].x,sprites[0].buff,sprites[0].replacePix,
         &(sprites[0].oldstartRow),&(sprites[0].oldstartCol));
         sprites[0].x+=10;
         spriteMove =0;
         sprites[0].replacePix=1;

	show_bmp(sprites[1].p, sprites[1].y, sprites[1].x,sprites[1].buff,sprites[1].replacePix,
         &(sprites[1].oldstartRow),&(sprites[1].oldstartCol));
         sprites[1].x+=10;
         spriteMove =0;
         sprites[1].replacePix=1;
      }*/
   
   }
}
