#include <stdio.h>
#include <stdlib.h>

int ch,addr,opcode,state,nstate=0;

static void address(int val)
{
  int offset;
  offset=val & 0177;
  if (val & 00400) printf("I ");
  if (val & 00200) offset += (addr & 07600);
  printf("%04o ",offset);
}

static void iot_decode(int val)
{
  switch(val) {
    case 06031: printf("KSF"); break;
    case 06032: printf("KCC"); break;
    case 06034: printf("KRS"); break;
    case 06036: printf("KRB"); break;
    case 06041: printf("TSF"); break;
    case 06042: printf("TCF"); break;
    case 06044: printf("TPC"); break;
    case 06046: printf("TLS"); break;
    case 06000: printf("SKON"); break;
    case 06001: printf("ION"); break;
    case 06002: printf("IOF"); break;
    default: printf("%04o",val);
  }
}

static void omi_decode(int val)
{
  int decoded=0;		// needed, in case we never find an opcode
  if (val & 00400) {	// group 2 OMIs
    if (val & 00010) {	// inverted sense bit
      if (val & 00100) { printf("SPA "); decoded=1; }
      if (val & 00040) { printf("SNA "); decoded=1; }
      if (val & 00020) { printf("SZL "); decoded=1; }
    } else {
      if (val & 00100) { printf("SMA "); decoded=1; }
      if (val & 00040) { printf("SZA "); decoded=1; }
      if (val & 00020) { printf("SNL "); decoded=1; }
    }
    // rest of group 2 OMIs
    if (val == 07410) { printf("SKP "); decoded=1; }	// I hate special cases
    if (val == 07401) { printf("NOP "); decoded=1; }	// I hate special cases
    if (val == 07402) { printf("HLT "); decoded=1; }	// I hate special cases
    if (val & 00200) { printf("CLA "); decoded=1; }
    if (val & 00004) { printf("OSR "); decoded=1; }
  } else {
    // group 1 OMIs
    if (val & 00200) { printf("CLA "); decoded=1; }
    if (val & 00100) { printf("CLL "); decoded=1; }
    if (val & 00040) { printf("CMA "); decoded=1; }
    if (val & 00020) { printf("CML "); decoded=1; }
    if (val & 00001) { printf("IAC "); decoded=1; }
    if (val & 00010 && !(val & 00002) ) { printf("RAR "); decoded=1; }
    if (val & 00010 && val & 00002) { printf("RTR "); decoded=1; }
    if (val & 00004 && !(val & 00002) ) { printf("RAL "); decoded=1; }
    if (val & 00004 && val & 00002) { printf("RTL "); decoded=1; }
  }
  if(!decoded) printf("%04o",val);
}

int main(int argc,char *argv[])
{
  int val;
  FILE *fptr;

  // get the arguments
  if (argc<2) {	// no argument given
    printf("%s: no input file\n",argv[0]);
    exit(1);
  }

  // open the input file
  if ((fptr=fopen(argv[1],"r")) == NULL) { // can't open the file
    printf("%s: couldn't open %s for reading\n",argv[0],argv[1]);
    exit(1);
  }
  state=0;	// before anything
  addr=0;
  val=0;
  while (((ch=getc(fptr)) != EOF) & (state!=6)) {
    if (ch==128) nstate=1;	// found leader
    if (state==1 && ch!=128) nstate=2;	// found real data
    if (state>0 && (ch & 0100)) {	// load addr;
      addr=(ch & 0077) << 6;
      nstate=3;
      state=1;

      }
    if (state==3) {			// load addr, lower half
      addr += (ch & 0077);
      printf("*%04o\n",addr);
      nstate=4;
      }
    if ((state==4)&&(ch==0200)) state=6;
    if (state==4) {
      val=ch << 6;
      nstate=5;
      }
    if (state==5) {
      val += ch;
      nstate=4;
    }
    if (state==5) {
//    printf("\t%04o\t%04o\t",addr);
      opcode=(val & 07000) >> 9;
      switch(opcode) {
        case 0: printf("AND "); address(val); break;
        case 1: printf("TAD "); address(val); break;
        case 2: printf("ISZ "); address(val); break;
        case 3: printf("DCA "); address(val); break;
        case 4: printf("JMS "); address(val); break;
        case 5: printf("JMP "); address(val); break;
	case 6: iot_decode(val); break;
        case 7: omi_decode(val); break;
      }
      addr++;
      printf("\n");
    }
    state=nstate;
  }
  printf("$\n");
  fclose(fptr);
  return(0);
}
