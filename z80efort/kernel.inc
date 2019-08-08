;.include "system.def"
;ERROR		.equ	0x0000

;
; Ports
;

;CTC		.equ	0x11
;SIOBD		.equ	0x1A		; data
;SIOBC		.equ	0x1B		; control
;PIOAD		.equ	0x1C		; data
;PIOAC		.equ	0x1D		; control
;PIOBD		.equ	0x1E		; data
;PIOBC		.equ	0x1F		; control

;; Memory allocation	0//code>--//--<name//up>--<sp//tib>--rp//em

;EM		.equ	0xFFFF		;top of RAM memory*******
;BM		.equ	0x0000		;bottom of ROM memory******
;COLDD		.equ	BM + 0x00	;cold start vector******

;UAB		.equ	0xC000		;user area base address*******
;UAEM		.equ	0xE200		;user area end of memory adr*******

;US		.equ	0x100		;user area size in cells
;RTS		.equ	0x100		;return stack/TIB size
;DTS		.equ	0x100		;data stack size

;SPP		.equ	UAEM - 0x02	;start of data stack (SP0)
;UPP		.equ	UAEM - 0x100	;start of user area (UP0)
;RPP		.equ	UAEM - 0x102	;start of return stack (RP0)
;TIBB		.equ	UAEM - 0x200	;terminal input buffer (TIB)
;NAMEE		.equ	BM + 0x1FFE	;initial name (ROM)dictionary*******
;CODEE		.equ	BM + 0x0000	;initial code (ROM)dictionary*******

;SiobRxQ		.equ	0xFB00		; one page circular queue
;SVAB		.equ	0xFE00		; System Variable Address Base
;IVEC		.equ	SVAB + 0x00	; system interrupt vector base address
;RxBAvlVec	.equ	SVAB + 0x04	; interrupt vector
;SRxCondVec	.equ	SVAB + 0x06	; interrupt vector
;RP		.equ	SVAB + 0x0C	; R-stack pointer, simulate RP reg
;UP		.equ	SVAB + 0x0E	; User area
;SiobRxQin	.equ	SVAB + 0x12	; SIOB Rx Q input pointer
;SiobRxQout	.equ	SVAB + 0x14	; SIOB Rx Q output pointer

;NextStep	.equ	0x14F		; back fill after penultimate assembly
;SiobRxBAvl	.equ	0x366		; back fill after penultimate assembly
;SiobSRxCond	.equ	0x377		; back fill after penultimate assembly
;COLD		.equ	0x143C		; back fill after penultimate assembly

;; The kernel

;   doLIT       ( -- w )
;               Push an inline literal.

                $CODE   COMPO+5,'doLIT',DOLIT
DB 0Ah		;LD  A, (BC)  ; 7t
DB 03h		;INC BC       ; 6t
DB 6Fh		;LD  L, A      ; 4t
DB 0Ah		;LD  A, (BC)   ; 7t
DB 03h		;INC BC       ; 6t
DB 67h		;LD  H, A      ; 4t
DB 0E5h		;PUSH HL      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t    ;(10MHz)5.5usec (+2.5usec NextStep)


;   doLIST      ( a -- )
;               Process colon list.

                $CODE   COMPO+6,'doLIST',DOLST
DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 2Bh		;DEC HL       ; 6t
DB 70h		;LD  (HL), B  ; 7t      \ end of r-push simulation
DB 2Bh		;DEC HL       ; 6t
DB 71h		;LD  (HL), C  ; 7t      \ end of r-push simulation
DB 22h,0Ch,0FEh	;LD  (RP), HL ;16t      \ IP is r-pushed (simulated)
;                    ;
DB 0C1h		;POP BC       ;10t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                      ;99t==9.9usec (+ 2.4usec for NextStep)



;   EXIT        ( -- )
;               Terminate a colon definition.

                $CODE   4,'EXIT',EXIT

DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 4Eh		;LD  C, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 46h		;LD  B, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 22h,0Ch,0FEh	;LD  (RP),HL  ;16t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;68t==6.8us



;   EXECUTE     ( ca -- )
;               Execute the word at ca.

                $CODE   7,'EXECUTE',EXECU
DB 0E1h		;POP HL        ;10t
DB 0E9h		;JP  (HL)      ; 4t



;   next        ( -- )
;               Run time code for the single index loop.
;               : next ( -- ) \ hilevel model
;                 r> r> dup if 1 - >r @ >r exit then drop cell+ >r ;

                $CODE   COMPO+4,'next',DONXT
DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 7Eh		;LD  A, (HL)  ; 7t
DB 0B7h		;OR  A        ; 4t
DB 20h,10h	;JR  NZ, DECLOW;12/7t a fast dec is ok, only failed every 255 time
                       ;                    ; low byte 0
DB 23h		;INC HL       ; 6t
DB 7Eh		;LD  A, (HL)  ; 7t
DB 0B7h		;OR  A        ; 4t
DB 20h,09h	;JR  NZ, DECHILO;12/7t Hi-byte no-zero, it is also a re-loop case
                       ;zero bound now .. .
DB 23h		;INC HL       ; 6tdiscard the loop count on R-stack
DB 22h,0Ch,0FEh	;LD  (RP), HL ;16t
DB 03h		;INC BC       ; 6t\ IP slip over the re-loop-addr
DB 03h		;INC BC       ; 6t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t loop is over
;                    ; 98t==(10MHz)9.8usec
;DECHILO:
DB 35h		;DEC (HL)     ;11t hi-byte
DB 2Bh		;DEC HL       ; 6t back to low byte
;DECLOW:
DB 35h		;DEC (HL)     ;11t low byte non-zero, just dec it and re-loop
DB 69h		;LD  L, C     ; 4t get loop-start-adr to IP and keep stepping
DB 60h		;LD  H, B     ; 4t
DB 4Eh		;LD  C, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 46h		;LD  B, (HL)  ; 7t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
; low byte dec:    88t==(10MHz)8.8usec
; lo&Hi byte dec: 134t==(10MHz)13.4usec


;   ?brAhnch     ( f -- )
;               Branch if flag is zero.

                $CODE   COMPO+7,'?branch',QBRAN
DB 0E1h		;POP HL        ;10t
DB 7Dh		;LD  A, L      ; 4t    ?branch adr is just after ?branch
DB 0B4h		;OR  H         ; 4t    and IP is pointing it
DB 28h,05h	;JR  Z,ZEROO   ; 12/7t (Z=1,12t)
DB 03h		;INC BC        ; 6t    IP slip over the retun addr
DB 03h		;INC BC        ; 6t    ex: 'TRUE IF' will slip over
DB 0C3h,4Fh,01h	;JP  NextStep  ;10t  ;47t==(10MHz)4.7usec
;
;ZEROO:
DB 69h		;LD  L, C      ; 4t    get IP :=[IP] and go
DB 60h		;LD  H, B      ; 4t
DB 4Eh		;LD  C, (HL)   ; 7t
DB 23h		;INC HL        ; 6t
DB 46h		;LD  B, (HL)   ; 7t
DB 0C3h,4Fh,01h	;JP  NextStep  ;10t  68t==(10MHz)6.8usec



;   branch      ( -- )
;               Branch to an inline address.

                $CODE   COMPO+6,'branch',BRAN
DB 69h		;LD  L, C      ; 4t   get IP :=[IP] and go
DB 60h		;LD  H, B      ; 4t
DB 4Eh		;LD  C, (HL)   ; 7t
DB 23h		;INC HL        ; 6t
DB 46h		;LD  B, (HL)   ; 7t
DB 0C3h,4Fh,01h	;JP  NextStep  ;10t
;                     ;38t==(10MHz)3.8usec



;   !           ( w a -- )
;               Pop the data stack to memory.

                $CODE   1,'!',STORE
DB 0E1h		;POP HL       ;10t
DB 0D1h		;POP DE       ;10t
DB 73h		;LD  (HL), E  ; 7t
DB 23h		;INC HL       ; 6t
DB 72h		;LD  (HL), D  ; 7t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;50t==(10Mhz)5.0 usec



;   @           ( a -- w )
;               Push memory location to the data stack.

                $CODE   1,'@',AT
DB 0E1h		;POP HL       ;10t
DB 5Eh		;LD  E, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 56h		;LD  D, (HL)  ; 7t
DB 0D5h		;PUSH DE      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;51t==(10Mhz)5.1usec



;   C!          ( c b -- )
;               Pop the data stack to byte memory.

                $CODE   2,'C!',CSTOR
DB 0E1h		;POP HL        ;10t
DB 0D1h		;POP DE        ;10t
DB 73h		;LD  (HL), E   ; 7t
DB 0C3h,4Fh,01h	;JP  NextStep  ;10t
;                     ;37t==(10Mhz)3.7usec




;   C@          ( b -- c )
;               Push byte memory location to the data stack.

                $CODE   2,'C@',CAT
DB 0E1h		;POP HL       ;10t
DB 5Eh		;LD  E, (HL)  ; 7t
DB 16h,00h	;LD  D, #0     ; 7t
DB 0D5h		;PUSH DE      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;45t==(10Mhz)4.5usec




;   >R          ( w -- )
;               Push the data stack to the return stack.

                $CODE   COMPO+2,'>R',TOR
DB 0D1h		;POP DE       ;10t
DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 2Bh		;DEC HL       ; 6t
DB 72h		;LD  (HL), D  ; 7t
DB 2Bh		;DEC HL       ; 6t
DB 73h		;LD  (HL), E  ; 7t
DB 22h,0Ch,0FEh	;LD  (RP), HL ;16t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;78t==(10MHz)7.8usec



;   R@          ( -- w )
;               Copy top of return stack to the data stack.

                $CODE   2,'R@',RAT
DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 5Eh		;LD  E, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 56h		;LD  D, (HL)  ; 7t
DB 0D5h		;PUSH DE      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;57t==(10MHz)5.7usec



;   R>          ( -- w )
;               Pop the return stack to the data stack.

                $CODE   2,'R>',RFROM
DB 2Ah,0Ch,0FEh	;LD  HL, (RP) ;16t
DB 5Eh		;LD  E, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 56h		;LD  D, (HL)  ; 7t
DB 23h		;INC HL       ; 6t
DB 22h,0Ch,0FEh	;LD  (RP), HL ;16t
DB 0D5h		;PUSH DE      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;67t==(10MHz)5.7.usec



;   RP@         ( -- a )
;               Push the current RP to the data stack.

                $CODE   3,'RP@',RPAT
DB 2Ah,0Ch,0FEh	;LD  HL, (RP)
DB 0E5h		;PUSH HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   RP!         ( a -- )
;               Set the return stack pointer.

                $CODE   COMPO+3,'RP!',RPSTO
DB 0E1h		;POP HL       ;
DB 22h,0Ch,0FEh	;LD  (RP), HL ;
DB 0C3h,4Fh,01h	;JP  NextStep ;



;   SP@         ( -- a )
;               Push the current data stack pointer.

                $CODE   3,'SP@',SPAT
DB 21h,00h,00h	;LD  HL, #0
DB 39h		;ADD HL, SP
DB 0E5h		;PUSH HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   SP!         ( a -- )
;               Set the data stack pointer.

                $CODE   3,'SP!',SPSTO
DB 0E1h		;POP HL
DB 0F9h		;LD  SP, HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   DUP         ( w -- w w )
;               Duplicate the top stack item.

                $CODE   3,'DUP',DUPP
DB 0E1h		;POP HL       ;10t
DB 0E5h		;PUSH HL      ;11t
DB 0E5h		;PUSH HL      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;42t==(10MHz)4.2usec



;   DROP        ( w -- )
;               Discard top stack item.

                $CODE   4,'DROP',DROP
DB 0E1h		;POP HL       ;10t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
;                    ;20t==(10MHz)2.0usec



;   SWAP        ( w1 w2 -- w2 w1 )
;               Exchange top two stack items.

                $CODE   4,'SWAP',SWAP
DB 0E1h		;POP HL       ;10t
DB 0E3h		;EX  (SP), HL ;19t
DB 0E5h		;PUSH HL      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
                       ;                    ;50t==(10MHz)5.0usec



;   OVER        ( w1 w2 -- w1 w2 w1 )
;               Copy second stack item to top.

                $CODE   4,'OVER',OVER
DB 0D1h		;POP DE       ;10t
DB 0E1h		;POP HL       ;10t
DB 0E5h		;PUSH HL      ;11t
DB 0D5h		;PUSH DE      ;11t
DB 0E5h		;PUSH HL      ;11t
DB 0C3h,4Fh,01h	;JP  NextStep ;10t
                       ;                    ;63t==(10MHz)6.3usec



;   0<          ( n -- t )
;               Return true if n is negative.

                $CODE   2,'0<',ZLESS
DB 0E1h		;POP HL         ;10t
DB 29h		;ADD HL, HL     ;11t
DB 38h,07h	;JR  C, LESSZ   ;12/7t
DB 21h,00h,00h	;LD  HL, 0      ;10t
DB 0E5h		;PUSH HL        ;11t
DB 0C3h,4Fh,01h	;JP  NextStep   ;10t   59t==(10MHz)5.9usec
                       ;
;LESSZ:
DB 21h,0FFh,0FFh	;LD  HL, 0xFFFF ;10t
DB 0E5h		;PUSH HL        ;11t
DB 0C3h,4Fh,01h	;JP  NextStep   ;10t   64t==(10MHz)6.4usec



;   AND         ( w w -- w )
;               Bitwise AND.

                $CODE   3,'AND',ANDD
DB 0D1h		;POP DE
DB 0E1h		;POP HL
DB 7Bh		;LD  A, E
DB 0A5h		;AND L
DB 6Fh		;LD  L, A
DB 7Ah		;LD  A, D
DB 0A4h		;AND H
DB 67h		;LD  H, A
DB 0E5h		;PUSH HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   OR          ( w w -- w )
;               Bitwise inclusive OR.

                $CODE   2,'OR',ORR
DB 0D1h		;POP DE
DB 0E1h		;POP HL
DB 7Bh		;LD  A, E
DB 0B5h		;OR  L
DB 6Fh		;LD  L, A
DB 7Ah		;LD  A, D
DB 0B4h		;OR  H
DB 67h		;LD  H, A
DB 0E5h		;PUSH HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   XOR         ( w w -- w )
;               Bitwise exclusive

                $CODE   3,'XOR',XORR
DB 0D1h		;POP DE
DB 0E1h		;POP HL
DB 7Bh		;LD  A, E
DB 0ADh		;XOR L
DB 6Fh		;LD  L, A
DB 7Ah		;LD  A, D
DB 0ACh		;XOR H
DB 67h		;LD  H, A
DB 0E5h		;PUSH HL
DB 0C3h,4Fh,01h	;JP  NextStep



;   UM+         ( w w -- w cy )
;               Add two numbers, return the sum and carry flag.

                $CODE   3,'UM+',UPLUS
DB 0D1h		;POP DE         ;10t
DB 0E1h		;POP HL         ;10t
DB 19h		;ADD HL, DE     ;11t
DB 0E5h		;PUSH HL        ;11t
DB 38h,07h	;JR  C, CARRY   ;12/7t
DB 21h,00h,00h	;LD  HL, #0      ;10t
DB 0E5h		;PUSH HL        ;11t
DB 0C3h,4Fh,01h	;JP  NextStep   ;10t
                       ;
;CARRY:
DB 21h,01h,00h	;LD  HL, #1      ;10t
DB 0E5h		;PUSH HL        ;11t
DB 0C3h,4Fh,01h	;JP  NextStep   ;10t
