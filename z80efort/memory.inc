;; Memory allocation    0//code>--//--<name//up>--<sp//tib>--rp//em

EM              EQU     0FFFFH          ;top of RAM memory*******
BM              EQU     0H              ;bottom of ROM memory******
COLDD           EQU     BM+00H          ;cold start vector******

UAEM            EQU    0E200H           ;user area end of memory adr*******

SPP             EQU     UAEM -   2H     ;start of data stack (SP0)
UPP             EQU     UAEM - 100H     ;start of user area (UP0)
RPP             EQU     UAEM - 102H     ;start of return stack (RP0)
TIBB            EQU     UAEM - 200H     ;terminal input buffer (TIB)
NAMEE           EQU     BM+1FFEH        ;initial name (ROM)dictionary*******
CODEE           EQU     BM+0            ;initial code (ROM)dictionary*******
