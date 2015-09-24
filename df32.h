#define DF32SIZE	262144	// size in words of DF32
#define DF32XFER	16	// number of words to transfer per cycle

#define DF32IDLE	0
#define DF32READ	1
#define DF32WRITE	2

#define PCS		04000	// Photo Cell Sync
int df32[DF32SIZE];
int df32_wc,df32_ca;		// disk word count and address registers
int df32_dmar,df32_ema;		// registers
int df32_pe_f,df32_f;		// flags
int df32_int_f;			// interrupt flags
int df32_action;		// what is the DF32 actually doing?

// function prototypes
void df32_iot0(int command);
void df32_iot2(int command);
void df32_init(void);
void df32_run(void);
