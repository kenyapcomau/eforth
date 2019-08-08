;.include "system.def"
;		System Variable Area  (SVA)  (not user woned)

; Ports
;CTC		.equ	0x11
;SIOBD		.equ	0x1A		; data
;SIOBC		.equ	0x1B		; control
;PIOAD		.equ	0x1C		; data
;PIOAC		.equ	0x1D		; control
;PIOBD		.equ	0x1E		; data
;PIOBC		.equ	0x1F		; control

;SiobRxQ		.equ	0xFB00		; one page circular queue
;SVAB		.equ	0xFE00		; System Variable Address Base
;IVEC		.equ	SVAB + 0x00	; system interrupt vector base address
;RxBAvlVec	.equ	SVAB + 0x04	; interrupt vector
;SRxCondVec	.equ	SVAB + 0x06	; interrupt vector
;RP		.equ	SVAB + 0x10	; R-stack pointer, simulate RP reg
;SiobRxQin	.equ	SVAB + 0x12	; SIOB Rx Q input  pointer
;SiobRxQout	.equ	SVAB + 0x14	; SIOB Rx Q output pointer

;NextStep	.equ	0x14F		; back fill after penultimate assembly
;SiobRxBAvl	.equ	0x366		; back fill after penultimate assembly
;SiobSRxCond	.equ	0x377		; back fill after penultimate assembly

;   PC!         ( uc p --  )
;               output uc to IO port p

                $CODE   3,'PC!',PCSTO

DB 0D9h		;EXX		; 4t
DB 0C1h		;POP BC		;10t
DB 0D1h		;POP DE		;10t
DB 7Bh		;LD A, E		; 4t
DB 0EDh,79h	;OUT (C), A	;12t
DB 0D9h		;EXX		; 4t
DB 0C3h,4Fh,01h	;JP NextStep	;10t
;54t==(10MHz)5.4usec


;   PC@         ( p -- uc )
;               input uc from IO port p

                $CODE   3,'PC@',PCAT

DB 0D9h		;EXX		; 4t
DB 0C1h		;POP BC		;10t
DB 0EDh,78h	;IN A, (C)	;12t
DB 5Fh		;LD E, A		; 4t
DB 16h,00h	;LD D, 0		; 7t
DB 0D5h		;PUSH DE		;11t
DB 0D9h		;EXX		; 4t
DB 0C3h,4Fh,01h	;JP NextStep	;10t
;62t==(10MHz)6.2usec
