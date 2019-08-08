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
;COLDSTART	.equ	0x100		;code starts here

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

DB 0C3h,00h,01h	;JP COLDSTART	;Jump to cold start  100H
;.rept 120	;should be 122, but we'll match the binary for now
;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 00h		;NOP
;.endm
DB 0C3h,00h,01h	;JP COLDSTART	;if something's wrong, jump again.
