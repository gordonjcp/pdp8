#define CORESIZE 8192	// core size in words

// Some variables we'll need
int core[CORESIZE];
int pc,ma,mb,ac,l,mq,sr;	// define registers
int cycles,halt;		// Cycle counter (needed for timing) and halt/run flag
int dfr,ifr,ifrb,i_dfr,i_ifr;	// memory field registers

// Some function prototypes
void preload();			// preload code into core
void rim();			// get a file in BIN format (misnomer really)
