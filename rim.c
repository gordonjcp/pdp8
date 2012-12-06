#include <stdio.h>
#include "pdp8.h"
void rim()
{
  int ch,addr,val,state,nstate=0;
  FILE *fopen(), *fptr;

  if ((fptr=fopen("in.bin","r")) == NULL)
    {
      printf("It's broken\n");
    }
  state=0;	// before anything  
  addr=0;
  val=0;
  while ((ch=getc(fptr)) != EOF) {
    if (ch & 128) { nstate=1; state=1; }	// found leader
    if (state==1 && ch!=128) nstate=2;	// found real data
    if (state>0 && (ch & 0100)) {	// load addr;
      printf("getting address - 1\n");
      addr=(ch & 0077) << 6;
      nstate=3;
      state=1;
      
      }
    if (state==3) {			// load addr, lower half
      printf("getting address - 2\n");
      addr += (ch & 0077);
      nstate=4;
      }
      
    if (state==4) {
      val=ch << 6;
      nstate=5;
      }
    if (state==5) {
      val += ch;
      core[addr]=val;
     printf("Addr: %04o Val: %04o\n",addr,val);
      addr++;
      nstate=4;
    }
    //if (state==0) printf("Reading preamble\n");
//    if (state==1) printf("Reading leader\n");
    
//    if (nstate>0) printf("State: %o ch: %03o Addr: %04o Val: %04o\n",state,ch,addr,val);

    state=nstate;
  }
  fclose(fptr);
}  
