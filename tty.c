#include <stdio.h>
#include <ncurses.h>
#include "pdp8.h"
#include "tty.h"

void tty_pun(int command)
{
  if ((command & 1) && tty_pun_f) pc++; // TSF - Skip if we're ready to print
  if (command & 2) {			// TCF - clear the punch flag
    tty_pun_f=0;
  }
  if (command & 4) {			// TPC - tell the punch to print
    tty_pun_buf=ac;			// there's actually some cooking to go on here
    tty_pun_clk=cycles+TTY_PUN_TIME;
  }
}

void tty_rdr(int command)
{
  if ((command & 1) && tty_rdr_f) pc++; // KSF - Skip if we've got a character
  if (command & 2) {			// KCC - Clear AC, clear keyboard flag
    tty_rdr_f=0;
    ac=0;
  }
  if (command & 4) { 			// KRB - read the reader buffer
    if (tty_rdr_buf == 00012) {
      tty_rdr_buf=00015;			// <RETURN> becomes CR
    }
    if (tty_rdr_buf == 00527) {
      tty_rdr_buf=00012;			// <KP_ENTER> becomes LF
    }
    ac=tty_rdr_buf | 00200; }		// always has MSB set
}

void tty_run()
{
  if (cycles > tty_pun_clk && tty_pun_clk!=0) {		//  We're ready to print
    tty_pun_clk = 0;
    tty_pun_f = 1;
    switch(tty_pun_buf) {
      case 0215: tty_col=0; break;	// carriage return
      case 0212: scroll(stdscr); break;	// line feed
      case 0377: tty_col--; break;	// backspace
      default:
        mvwprintw(stdscr,21,tty_col,"%c",tty_pun_buf & 00177);	// MSB needs masked off
	tty_col++;
    }
  }
}
