// DF32 unit

#include <stdio.h>
#include <ncurses.h>
#include "pdp8.h"
#include "df32.h"

int df32[DF32SIZE];
int df32_wc,df32_ca;		// disk word count and address registers
int df32_dmar,df32_ema;		// registers
int df32_pe_f,df32_f;		// flags
int df32_int_f;			// interrupt flags
int df32_action;		// what is the DF32 actually doing?


void df32_init()		// load a file into the DF32 buffer
{
  int i,ch,val = 0;
  FILE *fptr;

  if ((fptr=fopen("dms.df32","r")) == NULL) {
    printf("It's broken\n");
  }

  for (i=0; (ch=getc(fptr)) != EOF;i++) {
    if ((i & 1)== 0) {
      val=ch;
    } else {
      val += ch<<8;
  //    printf("%06o %04o\n",i>>1,val);
      df32[i>>1]=val;

    }
  }
  fclose (fptr);
  // reset all the registers etc
  df32_action=DF32IDLE;		// it's doing nothing
  df32_dmar=0;			// reset DMAR
  df32_pe_f=0; df32_f=0;	// reset flags
  df32_int_f=0;			// clear interrupt flags
  df32_ema=0;
}

void df32_iot0(int command)	// Handle the IOT for the DF32, device 60
{
  if (command & 1) {		// DCMA bit
    df32_dmar=0;		// Clear DMAR
    df32_pe_f=0;		// Clear Parity Error flag
    df32_f=0;			// Clear Complete flag
    df32_int_f=0;		// Clear interrupt flags
  }
  if (command & 2) {		// DMAR bit
    df32_dmar = ac;		// put AC into Data Memory Address reg
    ac=0;			// clear the AC
    df32_wc = core[07750];	// not sure if this needs to take IFR into account
    df32_ca = core[07751];	// or possibly DFR, who knows?
    df32_action=DF32READ;	// start the DF32 reading the disk
    printf("got read");
  }
  if (command & 4) {		// DMAW bit
    df32_dmar = ac;		// put AC into Data Memory Address reg
    ac=0;			// clear the AC
    df32_wc = core[07750];	// not sure if this needs to take IFR into account
    df32_ca = core[07751];	// or possibly DFR, who knows?
    df32_action=DF32WRITE;	// start the DF32 writing to the disk
    printf("got write");
  }
}

void df32_iot2(int command)
{
  if (command & 1) pc++;
  if ((command & 2) && df32_f) pc++;
  if (command & 4) pc=df32_dmar;
}

void df32_run()			// called every CPU cycle
{
  int i;
  if (df32_action != DF32IDLE) {
  for (i=0; i<DF32XFER; i++) {
    if (df32_action==DF32READ) {
      printf("reading...");
      core[df32_ca]=df32[(4096*(df32_ema>>1))+df32_dmar];
    }
//    if (df32_action==DF32WRITE) df32[(4096*(df32_ema>>1))+df32_dmar]=core[df32_ca];

  //printw("%04o %04o %06o %04o\n",df32_wc,df32_ca,df32_dmar,df32[(4096*(df32_ema>>1))+df32_dmar]);
    df32_ca++; df32_dmar++; df32_wc++;
    if (df32_wc==010000) {
      printw("%o ",df32_action);

      i=DF32XFER;
      df32_f=1;
      df32_action=DF32IDLE;
    }
  }
  }
}
