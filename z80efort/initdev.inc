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

;EM		.equ	0xFFFF		;top of RAM memory
;BM		.equ	0x0000		;bottom of ROM memory
;COLDD		.equ	BM + 0x00	;cold start vector

;UAB		.equ	0xC000		;user area base address
;UAEM		.equ	0xE200		;user area end of memory address

;US		.equ	0x100		;user area size in cells
;RTS		.equ	0x100		;return stack/TIB size
;DTS		.equ	0x100		;data stack size

;SPP		.equ	UAEM - 0x02	;start of data stack (SP0)
;UPP		.equ	UAEM - 0x100	;start of user area (UP0)
;RPP		.equ	UAEM - 0x102	;start of return stack (RP0)
;TIBB		.equ	UAEM - 0x200	;terminal input buffer (TIB)
;NAMEE		.equ	BM + 0x1FFE	;initial name (ROM)dictionary
;CODEE		.equ	BM + 0x0000	;initial code (ROM)dictionary

;SiobRxQ		.equ	0xFB00		; one page circular queue
;SVAB		.equ	0xFE00		; System Variable Address Base
;IVEC		.equ	SVAB + 0x00	; system interrupt vector base address
;RxBAvlVec	.equ	SVAB + 0x04	; interrupt vector
;SRxCondVec	.equ	SVAB + 0x06	; interrupt vector
;RPA		.equ	SVAB + 0x0C	; R-stack pointer, simulate RP reg
;UPA		.equ	SVAB + 0x0E	; User area
;SiobRxQin	.equ	SVAB + 0x12	; SIOB Rx Q input pointer
;SiobRxQout	.equ	SVAB + 0x14	; SIOB Rx Q output pointer

;NextStep	.equ	0x14F		; back fill after penultimate assembly
;SiobRxBAvl	.equ	0x366		; back fill after penultimate assembly
;SiobSRxCond	.equ	0x377		; back fill after penultimate assembly
;COLD		.equ	0x143C		; back fill after penultimate assembly
; 
;
;; Device dependent I/O
;

;   !IO         ( -- )
;               Initialize the serial I/O devices.

                $CODE   3,'!IO',STOIO ;

;TST:			; init SIO interrupt vector
DB 0D9h		;EXX			; 4t
DB 3Eh,0FEh	;LD A, 0x0FE
DB 0EDh,47h	;LD I, A
DB 21h,66h,03h	;LD HL, #SiobRxBAvl
DB 22h,04h,0FEh	;LD (RxBAvlVec), HL	; interrupt vector cell
DB 21h,77h,03h	;LD HL, #SiobSRxCond
DB 22h,06h,0FEh	;LD (SRxCondVec), HL	; interupt vector cell
;
;; init SIOB Rx Q ptr
;
DB 21h,00h,0FBh	;LD HL, #SiobRxQ
DB 22h,12h,0FEh	;LD (SiobRxQin), HL
DB 22h,14h,0FEh	;LD (SiobRxQout), HL
;
;; init CTC
;
DB 0Eh,11h	;LD C, #CTC
DB 3Eh,07h	;LD A, 0x07
DB 0EDh,79h	;OUT (C), A
DB 3Eh,04h	;LD A, 0x04
DB 0EDh,79h	;OUT (C), A
;
;; init PIO
;
DB 0Eh,1Dh	;LD C, #PIOAC
DB 3Eh,0CFh	;LD A, 0xCF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD A, 0x0FF
DB 0EDh,79h	;OUT (C), A
;
DB 0Eh,1Fh	;LD C, #PIOBC
DB 3Eh,0CFh	;LD A, 0x0CF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD A, 0x0FF
DB 0EDh,79h	;OUT (C), A
;
DB 0Eh,1Ch	;LD C, #PIOAD
DB 3Eh,3Bh	;LD A, 0x3B		;PIOA data: TST & Green LED
DB 0EDh,79h	;OUT (C), A
;HALT			; one of testing poing during porting !!!!!
DB 0Eh,1Eh	;LD C, #PIOBD
DB 3Eh,17h	;LD A, 0x17 		;PIOB data: .. .
DB 0EDh,79h	;OUT (C), A
;
;; init SIO
;
DB 0Eh,1Bh	;LD C, #SIOBC
DB 3Eh,18h	;LD A, 0x18
DB 0EDh,79h	;OUT (C), A
DB 3Eh,01h	;LD A, 0x01
DB 0EDh,79h	;OUT (C), A
DB 3Eh,1Dh	;LD A, 0x1D
DB 0EDh,79h	;OUT (C), A
;
DB 3Eh,02h	;LD A, 0x02
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD A, 0x00
DB 0EDh,79h	;OUT (C), A
;
DB 3Eh,03h	;LD A, 0x03
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0C1h	;LD A, 0xC1
DB 0EDh,79h	;OUT (C), A
;
DB 3Eh,04h	;LD A, 0x04
DB 0EDh,79h	;OUT (C), A
DB 3Eh,44h	;LD A, 0x44
DB 0EDh,79h	;OUT (C), A
DB 3Eh,05h	;LD A, 0x05
DB 0EDh,79h	;OUT (C), A
;
DB 3Eh,68h	;LD A, 0x68
DB 0EDh,79h	;OUT (C), A
;
DB 0FBh		;EI
DB 00h		;NOP
DB 00h		;NOP
DB 00h		;NOP
;LD  C, #PIOBC
;LD  A, 0x01		;off-hook
;OUT (C), A
;HALT			; !!!!!
DB 0D9h		;EXX			; 4t
DB 0C3h,4Fh,01h	;JP NextStep
