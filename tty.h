#ifndef _TTY_H
#define _TTY_H

#define TTY_PUN_TIME 50  // 50 clock periods for each punched character
                         // we can't just let the TTY print all the time
                         // otherwise things that rely on interrupts will fail a bit
// prototypes for the TTY functions
extern void tty_rdr(int);  //
extern void tty_pun(int);
extern void tty_run(void);

// some variables that the TTY will use
extern int tty_rdr_buf, tty_pun_buf;  // TTY buffers
extern int tty_pun_f, tty_rdr_f;      // TTY flags
extern int tty_pun_clk;               // Contains the next cycle when the flag will be cleared
extern int tty_col;

#endif