#ifndef _PDP8_H
#define _PDP8_H
#define CORESIZE 8192	// core size in words

// Some variables we'll need
extern int core[CORESIZE];
extern int pc,ma,mb,ac,l,mq,sr;	// define registers
extern int cycles,halt;		// Cycle counter (needed for timing) and halt/run flag
extern int dfr,ifr,ifrb,i_dfr,i_ifr;	// memory field registers

// Some function prototypes
extern void preload(void);			// preload code into core
extern void rim(void);			// get a file in BIN format (misnomer really)
#endif