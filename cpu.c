/* PDP-8 emulator */

#include <stdio.h>
#include "pdp8.h"
#include "cpu.h"
#include "tty.h"
#include "df32.h"

// getaddr() - get the address an opcode refers to
int getaddr(int addr)
{
  int offset;

  offset = addr & 00177;	// size of a page
  // is the address on the current page?
  if (addr & 00200) offset = offset + (pc & 07600);
  // is it indirect?
  if (addr & 00400) offset = core[offset];

  // is it an auto-incrementing register?
  if (addr & 00400 && offset <= 00017 && offset >= 00010) core[offset]++;
  return (offset);
}

static void rar(void)
{
  ac += (l*010000);	// l gets ready to be rotated to bit 0 (now it's at bit -1!)
  l=ac & 00001;		// l now takes the bit that's about to fall off the end
  ac=ac>>1;		// bump one place right;
}
static void ral(void)
{
  int temp;
  temp=l;
  l=(ac & 04000)/04000;	// shifts might make that faster
  ac=((ac << 1) & 07777) | temp; // right, nudge one place left and lop off the top bit, then or with l
}

// functionmesh() - decode a function
int functionmesh(int opcode)
{
  int func,addr;

  func = (opcode & 07000) >> 9;	// get the actual function
  addr = getaddr(opcode)+dfr*010000;	// get the address we're working on
  switch(func) {
    case 0: {			// AND (bitwise and AC and core)
      ac = ac & core[addr];
      pc++;
      break;
    }
    case 1: {			// TAD (two's complement add AC and core)
      int temp;
      temp = ac + core[addr];
      if (temp & 0170000) l=!l;	// it's overflowed
      ac = temp & 0007777;	// make sure it's never more than 12 bits
      pc++;
      break;
    }
    case 2: {			// ISZ (increment core, skip if zero)
      int temp;
      temp=(core[addr]+1) & 07777;
      if (temp == 0) pc++; // skip if zero
      core[addr]=temp;
      pc++;
      break;
    }
    case 3: {			// DCA (deposit AC, clear AC)
      core[addr]=ac;
      ac=0;
      pc++;
      break;
    }
    case 4: {			// JMS (jump to subroutine)
      core[addr] = ++pc;	// return address is bumped by one
      ifr=ifrb;			// Instruction Field
      pc = ++addr;		// jump to one after the jump address
      break;
    }
    case 5: {			// JMP (jump to address)
      ifr=ifrb;			// Instruction Field
      pc=addr;			// very simple
      break;
    }
    case 6: {			// IOTs
      int device = (opcode & 00770) >> 3;	// get the device code

      int command = opcode & 00007;	// get the command for the device
      switch (device) {
        case 000: break;		// Interrupts
	case 001: break;		// High speed reader
	case 002: break;		// High speed punch
	case 003: tty_rdr(command); break;		// Teletype keyboard
	case 004: tty_pun(command); break;		// Teletype punch
	case 060: df32_iot0(command); break;		// Disk DF32
	case 061: break;		// Disk DF32
	case 062: df32_iot2(command); break;		// Disk DF32
	case 075: break;		// Disk RX01
	default:
	  if ((opcode & 07700) == 06200){	// extended memory
	    int field = (opcode & 00070)>>3;
    	    if (command == 1) dfr=(opcode & 00070)>>3;	// Data Field Register
	    if (command == 2) ifrb=(opcode & 00070)>>3; // new Instruction field in buffer
	    if (command == 4) {
	      if (field == 1) { ac=ac & 07707; ac=ac | (dfr << 3); }
	      if (field == 2) { ac=ac & 07707; ac=ac | (ifr << 3); }
	      if (field == 3) { ac=ac & 0770; ac=ac | (ifr << 3) | dfr; }
	      if (field == 4) { ifrb=i_ifr; dfr=i_dfr; }
	    }
	  }
      }
      pc++;
      break;
    }
    case 7: {			// Operate microinstructions
      if (opcode & 00400) {	// group 2 OMIs
        int skip = 0, bit8=0;
      // if bits 5,6 or 7 are 1, skip - unless bit 8 is a 1, skip if they're 0
        if (opcode & 00010) bit8=1;	// invert the appropriate bits
//	printf("%o %o\n",bit8,((ac & 04000)!=0) ^ bit8 );
//	printf("%o %o\n",bit8,((ac ==0) ^ bit8 ));
	if ((opcode & 00100) && ((ac & 04000)!=0)  ^ bit8) skip=1;
	if ((opcode & 00040) && (ac==0) ^ bit8) skip=1;
	if ((opcode & 00020) && (l==0) ^ bit8) skip=1;
	if (opcode & 00200) ac=0;
	if (opcode & 00004) ac = ac | sr;
	if (opcode & 00002) halt=1;
	if (skip) pc++;
      } else {
        if (opcode & 00200) ac=0;
        if (opcode & 00100) l=0;
        if (opcode & 00040) ac=(07777-ac) & 07777;
        if (opcode & 00020) l=!l;
        if (opcode & 00001) ac=(ac+1) & 07777;
        if (opcode & 00010) rar();
        if (opcode & 00010 && opcode & 00002) rar();	// do it twice
        if (opcode & 00004) ral();
        if (opcode & 00004 && opcode & 00002) ral();	// again, do it twice
      }
      pc++;
      break;
    }
  }
  return addr;
}
