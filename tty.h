#define TTY_PUN_TIME 50		// 50 clock periods for each punched character
				// we can't just let the TTY print all the time
				// otherwise things that rely on interrupts will fail a bit
// prototypes for the TTY functions				
void tty_rdr(int);		// 
void tty_pun(int);
void tty_run();

// some variables that the TTY will use
int tty_rdr_buf,tty_pun_buf;		// TTY buffers
int tty_pun_f, tty_rdr_f;	// TTY flags
int tty_pun_clk;		// Contains the next cycle when the flag will be cleared
int tty_col;
