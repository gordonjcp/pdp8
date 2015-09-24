pdp8
====

PDP8 emulator I wrote about fifteen years ago

Since cleaned up to compile on modernish GCC with no warnings, although there is
some horribly ugly code in there.

Function keys used to control it are:

  F12 - exit
  F2  - single-step an instruction
  F3  - Halt/Run switch
  F4  - comments says "Interrupt", possibly fires the hardware interrupt?
  F9  - sets machine to halted at start of RIM loader

No attempt is made to be cycle-accurate
Autoindex isn't implemented
Patches are welcome :-)

Reading this in 2015 is like digging out an old schoolbook and wondering what
the hell you were thinking when you submitted that as English homework.
