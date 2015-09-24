#include "pdp8.h"

// preload()
// loads some example stuff into core
// possibly useful for putting a boot loader there
void preload()
{
  core[00200]=06603;
  core[00201]=06622;
  core[00202]=05201;
  core[00203]=05604;
  core[00204]=07600;
  core[07750]=07576;
  core[07751]=07576;

  core[05000]=07300;	// cla cll
  core[05001]=01170;	// tad buff
  core[05002]=03171;	// dca buffpt
  core[05003]=06031;	// ksf
  core[05004]=05203;	// jmp .-1
  core[05005]=06036;	// krb
  core[05006]=06046;	// tls
  core[05007]=03571;	// dca buffpt
  core[05010]=01571;	// tad buffpt
  core[05011]=01172;	// tad mdolar
  core[05012]=07450;	// sna
  core[05013]=05216;	// jmp done
  core[05014]=02171;	// isz buffpt
  core[05015]=05203;	// jmp lisn
  core[05016]=07300;	// cla cll
  core[05017]=05217;	// jmp .
  core[05170]=02000;	// BUFF
  core[05172]=07534;

  core[04000]=07300;
  core[04001]=06046;
  core[04002]=01300;
  core[04003]=03370;
  core[04004]=01371;
  core[04005]=03372;
  core[04006]=01770;
  core[04007]=04214;
  core[04010]=02370;
  core[04011]=02372;
  core[04012]=05206;
  core[04013]=05213;
  core[04014]=00000;
  core[04015]=06041;
  core[04016]=05215;
  core[04017]=06046;
  core[04020]=07200;
  core[04021]=05614;
  core[04100]=04101;
  core[04101]=00310;
  core[04102]=00305;
  core[04103]=00314;
  core[04104]=00314;
  core[04105]=00317;
  core[04106]=00241;
  core[04171]=-6;
  core[04172]=0;

  core[01000]=06211;
  core[01001]=07300;
  core[01002]=01202;
  core[01003]=05302;
  core[011002]=00400;
}
