; System Variable Area  (SVA)  (not user woned)

SVAB            EQU     0FE00H ; System Variable Address Base*******

IVEC            EQU     SVAB + 0H; system interrupt vector base address*******
RP              EQU     SVAB +10H; R-stack pointer, simulate RP reg*******
SiobRxQin       EQU     SVAB +12H; SIOB   Rx Q input  pointer*******
SiobRxQout      EQU     SVAB +14H; SIOB   Rx Q output pointer*******

SiobRxQ         EQU     0FB00H ; one page circular queue*******
