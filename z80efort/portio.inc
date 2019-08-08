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
