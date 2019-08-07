;; Device dependent I/O

;   !IO         ( -- )
;               Initialize the serial I/O devices.

                $CODE   3,'!IO',STOIO ;
;TST:           ; init  SIO interrupt vector
DB 0D9h		;EXX           ; 4t
DB 3Eh,0FEh	;LD  A, 0x0FE
DB 0EDh,47h	;LD  I, A
DB    21h
DW    SiobRxBAvl       ;       LD  HL, SiobRxBAvl
DB 22h,04h,0FEh	;LD  (0x0FE04), HL   ; intrrupt vector cell
DB    21h              ;       LD  HL, SiobSRxCond
DW    SiobSRxCond
DB 22h,06h,0FEh	;LD  (0x0FE06), HL   ; intrrupt vector cell
                       ;
                       ;; init SIOB Rx Q ptr
DB    21h, 00h,0FBh   ;       LD  HL, SiobRxQ
DB    22h, 12h,0FEh   ;       LD  (SiobRxQin), HL
DB    22h, 14h,0FEh   ;       LD  (SiobRxQout), HL
                       ;
                       ; ; init CTC
DB 0Eh,11h	;LD  C, 0x11  ; CTC
DB 3Eh,07h	;LD  A, 0x07
DB 0EDh,79h	;OUT (C), A
DB 3Eh,04h	;LD  A, 04
DB 0EDh,79h	;OUT (C), A
                       ;
                       ; ; init HW for my system Z80 PIO
DB 0Eh,1Dh	;LD  C, 0x1D  ; PIOA
DB 3Eh,0CFh	;LD  A, 0x0CF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD  A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD  A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD  A, 0x0FF
DB 0EDh,79h	;OUT (C), A
                       ;
DB 0Eh,1Fh	;LD  C, 0x1F   ;PIOB
DB 3Eh,0CFh	;LD  A, 0x0CF
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD  A, 0
DB 0EDh,79h	;OUT (C), A
DB 3Eh,17h	;LD  A, 0x17
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0FFh	;LD  A, 0x0FF
DB 0EDh,79h	;OUT (C), A

DB 0Eh,1Ch	;LD  C, 0x1C
DB 3Eh,3Bh	;LD  A, 0x3B ;PIOA data: TST & Green LED
DB 0EDh,79h	;OUT (C), A
;DB  0x76 ;!!!!!!!!!! HALT                ; one of testing poing during porting
DB 0Eh,1Eh	;LD  C, 0x1E
DB 3Eh,17h	;LD  A, 0x17     ;PIOB data: .. .
DB 0EDh,79h	;OUT (C), A
                       ;
                       ;;  init SIO
DB 0Eh,1Bh	;LD  C, 0x1B
DB 3Eh,18h	;LD  A, 0x18
DB 0EDh,79h	;OUT (C), A
DB 3Eh,01h	;LD  A, 0x01
DB 0EDh,79h	;OUT (C), A
DB 3Eh,1Dh	;LD  A, 0x1D
DB 0EDh,79h	;OUT (C), A
                       ;
DB 3Eh,02h	;LD  A, 0x02
DB 0EDh,79h	;OUT (C), A
DB 3Eh,00h	;LD  A, 0x00
DB 0EDh,79h	;OUT (C), A
                       ;
DB 3Eh,03h	;LD  A, 0x03
DB 0EDh,79h	;OUT (C), A
DB 3Eh,0C1h	;LD  A, 0xC1
DB 0EDh,79h	;OUT (C), A
                       ;
DB 3Eh,04h	;LD  A, 0x04
DB 0EDh,79h	;OUT (C), A
DB 3Eh,44h	;LD  A, 0x44
DB 0EDh,79h	;OUT (C), A
DB 3Eh,05h	;LD  A, 0x05
DB 0EDh,79h	;OUT (C), A
                       ;
DB 3Eh,68h	;LD  A, 0x68
DB 0EDh,79h	;OUT (C), A
                       ;
DB 0FBh		;EI
DB 00h		;NOP
DB 00h		;NOP
DB 00h		;NOP
;DB    0Eh, 1Eh         ;       LD  C, 1Eh
;DB    3Eh, 01h         ;       LD  A, 01h  ;off-hook
;DB   0EDh, 79h         ;       OUT (C), A
;DB  76H ;!!!!!!!!!! HALT                ;
DB 0D9h		;EXX           ; 4t
DB   0C3h
DW   NextStep          ;       JP NextStep
