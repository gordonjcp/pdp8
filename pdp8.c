#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "pdp8.h"
#include "tty.h"
#include "cpu.h"
#include "df32.h"
int main(void)
{
  int ch;
  WINDOW *reg_win;
  const char *runhalt[]={"Running","Halted ","Trap   "};

  preload();
  rim();

  df32_init();

  // get ncurses going
  initscr();
  raw();			// turn off line buffering
  noecho();
  keypad(stdscr,TRUE);		// lets us use function keys etc
  scrollok(stdscr,TRUE);
  reg_win=newwin(1,79,23,0);	// define a register window

  nodelay(stdscr, TRUE);	// nonblocking tty IO

  // set up the registers and stuff
  l=0;ac=0;			// AC and Link
  pc=07000;			// PC is set for ODT HIGH
  sr=0;				// SR is clear
  ifr=0; ifrb=0; dfr=0;		// Extended memory registers

  tty_pun_f=1;			// TTY flags preset
  tty_rdr_f=0;
  tty_pun_clk=0;
  cycles=0;			// cycle count
  halt=0;

  // initial display code goes here
  mvwprintw(reg_win,0,0,"PC: %05o L: %o AC: %04o MB: %04o %s", (010000*ifr)+pc,l,ac,core[(010000*ifr)+pc],runhalt[halt]);
  wrefresh(reg_win);

  // main loop begins
  while(ch!=KEY_F(12)) { 	// if the key pressed was F11, we'll leave
    ch = getch();		// get a keystroke
    if (ch!=ERR) {		// there actually *is* a keystroke
      switch (ch) {
        case KEY_F(2):		// F2 is single-step
	  functionmesh(core[pc+(010000*ifr)]);	// do the instruction
  	  break;
        case KEY_F(3):		// F3 is halt/run
	  halt=!halt;		// pretty simple really
	  break;
	case KEY_F(4):		// interrupt (sssh, nasty cheat)
	  core[00000]=pc+1;
	  pc=00001;
	  break;
	case KEY_F(9):
	  pc=07000;
	  halt=0;
	  break;
	default:
          tty_rdr_buf=ch;		// keep that ready for later, in the reader buffer
          tty_rdr_f=1;		// raise the flag, there's a character ready
      }
      // Eeeew.  This bit is horrible.  It prints the registers when we single-step
      // it's actually done every time we press a key.  Ick.  I hate this bit
      // in fact, let's have a *** FIXME *** right here
      mvwprintw(reg_win,0,0,"PC: %05o L: %o AC: %04o MB: %04o %s", (010000*ifr)+pc,l,ac,core[(010000*ifr)+pc],runhalt[halt]);
      wrefresh(reg_win);
    }

    cycles++;			// increase the cycle counter

    if(!halt) {			// CPU is running, let's do the instruction
      functionmesh(core[pc+(01000*ifr)]);
    }				// we can always add more here later

    tty_run();			// handle the TTY
    df32_run();
    // more peripheral handlers will go here
  }

  endwin();			// leave ncurses mode
  return (0);
}
