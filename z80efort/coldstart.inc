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

ORG BM + 100H


DB 0F3h		;DI
;
; move ROM version user data to 1st user RAM
;
DB 0Eh,1Dh	;LD C, #PIOAC		;PIOA
DB 3Eh,0CFh	;LD A, 0xCF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD A, 0xFF
DB 0EDh,79h	;OUT (C), A
;
DB 0Eh,1Fh	;LD C, #PIOBC		;PIOB
DB 3Eh,0CFh	;LD A, 0xCF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD A, 0xFF
DB 0EDh,79h	;OUT (C), A
;
DB 0Eh,1Ch	;LD C, #PIOAD
DB 3Eh,5Bh	;LD A, 0x5B		;PIOA data: TEST ON, RED LED on .. .
DB 0EDh,79h	;OUT (C), A
;
DB 0Eh,1Eh	;LD C, #PIOBD
DB 3Eh,17h	;LD A, 0x17		;PIOB data: .. .
DB 0EDh,79h	;OUT (C), A
;
; Set IM2
DB 0EDh,5Eh	;IM 2
;
; init SP, RP, UP
;
DB 21h,0FEh,0E1h	;LD HL, #SPP
DB 0F9h		;LD SP, HL		; INIT SP
;
DB 21h,0FEh,0E0h	;LD HL, #RPP		; init to RPP
DB 22h,0Ch,0FEh	;LD (RPA), HL		; simulated R-stack reg at RPA
;
DB 21h,00h,0E1h	;LD HL, #UPP		; use the initial USER VARIABLE area
DB 22h,0Eh,0FEh	;LD (UPA), HL		; user area at UPA
;
DB 01h,00h,00h	;LD BC, #ERROR		;jmp 0 cold-start whenever R-stack underflow-used
; this is supposed not impossible to be happen

DB 0Eh,1Ch	;LD C, #PIOAD
DB 3Eh,0BBh	;LD A, 0xBB		;PIOA data: GRN LED only .
DB 0EDh,79h	;OUT (C), A

DB 0C3h,3Ch,14h	;JP COLD
