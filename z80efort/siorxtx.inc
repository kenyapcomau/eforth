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

;   ?RX         ( -- c T | F )
;               Return input character and true, or a false if no input.

                $CODE   3,'?RX',QRX

DB 21h,14h,0FEh	;LD HL, #SiobRxQout	;16t
DB 7Eh		;LD A, (HL)		; 7t
DB 21h,12h,0FEh	;LD HL, #SiobRxQin	;16t
DB 0BEh		;CP (HL)			; 7t
DB 20h,07h	;JR NZ,$1		; 7t/12t (NZ jump=12t) ptr <>, get the char
DB 21h,00h,00h	;LD HL, 0		;10t False flag
DB 0E5h		;PUSH HL			;11t
DB 0C3h,4Fh,01h	;JP NextStep		;10t ==> False Timing:84t==(10MHz)8.4usec
;
;$1:
DB 5Eh		;LD E, (HL)		; 7t get this ptr LOW
DB 23h		;INC HL			; 6t
DB 56h		;LD D, (HL)		; 7t get this ptr HI
DB 0EBh		;EX DE,HL		; 4t let HL point the char
DB 5Eh		;LD E, (HL)		; 7t get the char
DB 16h,00h	;LD D, 0			; 7t high byte =0
DB 0D5h		;PUSH DE			;11t
DB 2Ch		;INC L			; 4t ptr+1, a 256 byte Ring queue
DB 22h,12h,0FEh	;LD (SiobRxQin),HL	;16t
DB 21h,0FFh,0FFh	;LD HL,0xFFFF		;10t
DB 0E5h		;PUSH HL			;11t
DB 0C3h,4Fh,01h	;JP NextStep		;10t ==> TRUE case: 100t(10MHz)10.0usec


;
;   TX!         ( c -- )
;               Send character c to the output device.

                $COLON  3,'TX!',TXSTO

TX1             DW      DOLIT, 0, DOLIT, 01BH, PCSTO	; BEGIN 0 $1B PC!
                DW      DOLIT, 1BH, PCAT		;         $1B PC@
                DW      DOLIT, 4, ANDD		;       4 AND
                DW      QBRAN,TX1			; UNTIL
                DW      DOLIT, 01AH, PCSTO, EXIT	; $1A PC! ;

;SiobRxBAvl:
; for timing: interrupt will get vector, push pc, jump to service .. .
;  a rough estimate:           7t   +       11t  +  10t

DB 0F5h		;PUSH AF			;11t ;save
DB 0E5h		;PUSH HL			;11t ;save
DB 0DBh,1Ah	;IN A, (0x1a)		;11t ;SIOB data reg
DB 2Ah,14h,0FEh	;LD HL, (SiobRxQout)	;16t
DB 77h		;LD (HL), A		; 7t ; put to ring queue
DB 2Ch		;INC L			; 4t ; 256 byte ring queue
DB 22h,14h,0FEh	;LD (SiobRxQout),HL	;16t ;restore
DB 0E1h		;POP HL			;10t ;restore
DB 0F1h		;POP AF			;10t ;restore
DB 0FBh		;EI			; 4t
DB 0EDh,4Dh	;RETI			;14t
;114t + (entering 28t) => 142t==(10MHz)14.2usec



SiobSRxCond:		; SIOB Special Rx Condition, reset only
DB 0F5h		;PUSH AF			; NOTE: a flag may be set for discard a process
DB 0C5h		;PUSH BC
DB 3Eh,04h	;LD A, 4
DB 0Eh,1Bh	;LD C, 0x1B
DB 0EDh,79h	;OUT (C), A		; reset
DB 3Eh,0FBh	;LD A, 0xFB
DB 0Eh,1Ch	;LD C, 0x1C
DB 0EDh,79h	;OUT (C), A
DB 0C1h		;POP BC
DB 0F1h		;POP AF
DB 0FBh		;EI
DB 0EDh,4Dh	;RETI
