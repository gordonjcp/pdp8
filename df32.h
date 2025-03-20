#ifndef _DF32_H
#define _DF32_H

#define DF32SIZE	262144	// size in words of DF32
#define DF32XFER	16	// number of words to transfer per cycle

#define DF32IDLE	0
#define DF32READ	1
#define DF32WRITE	2

#define PCS		04000	// Photo Cell Sync

// function prototypes
extern void df32_iot0(int command);
extern void df32_iot2(int command);
extern void df32_init(void);
extern void df32_run(void);

#endif