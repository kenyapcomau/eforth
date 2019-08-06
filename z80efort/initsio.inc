;; Device dependent I/O

;   !IO         ( -- )
;               Initialize the serial I/O devices.

                $CODE   3,'!IO',STOIO ;
TST:           ; init  SIO interrupt vector
DB   0D9h              ;       EXX           ; 4t
DB    3Eh,0FEh         ;       LD  A, 0FEH
DB   0EDh, 47h         ;       LD  I, A
DB    21h
DW    SiobRxBAvl       ;       LD  HL, SiobRxBAvl
DB    22h, 04h,0FEh    ;       LD  (0FE04H), HL   ; intrrupt vector cell
DB    21h              ;       LD  HL
DW    SiobSRxCond
DB    22h, 06h,0FEh    ;       LD  (0FE06H), HL   ; intrrupt vector cell
                       ;
                       ;; init SIOB Rx Q ptr
DB    21h, 00h,0FBh    ;       LD  HL, SiobRxQ
DB    22h, 12h,0FEh    ;       LD  (SiobRxQin), HL
DB    22h, 14h,0FEh    ;       LD  (SiobRxQout), HL
                       ;
                       ; ; init CTC
DB    0Eh, 11h         ;       LD  C, 011H   ; CTC
DB    3Eh, 07h         ;       LD  A, 07H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 04h         ;       LD  A, 04
DB   0EDh, 79h         ;       OUT (C), A
                       ;
                       ; ; init HW for my system Z80 PIO
DB    0Eh, 1Dh         ;       LD  C, 01DH   ; PIOA
DB    3Eh,0CFh         ;       LD  A, 0CFH
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 00h         ;       LD  A, 0
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 17h         ;       LD  A, 17H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh,0FFh         ;       LD  A, 0FFH
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    0Eh, 1Fh         ;       LD  C, 1FH   ;PIOB
DB    3Eh,0CFh         ;       LD  A, 0CFH
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 00h         ;       LD  A, 0
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 17h         ;       LD  A, 17H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh,0FFh         ;       LD  A, 0FFH
DB   0EDh, 79h         ;       OUT (C), A

DB    0Eh, 1Ch         ;check  LD  C, 1CH
DB    3Eh, 3Bh         ;check  LD  A, 03BH  ;PIOA data: TST & Green LED
DB   0EDh, 79h         ;check  OUT (C), A
;DB  76H ;!!!!!!!!!! HALT                ; one of testing poing during porting
DB    0Eh, 1Eh         ;       LD  C, 1EH
DB    3Eh, 17h         ;       LD  A, 17H     ;PIOB data: .. .
DB   0EDh, 79h         ;       OUT (C), A
                       ;
                       ;;  init SIO
DB    0Eh, 1Bh         ;       LD  C, 1BH
DB    3Eh, 18h         ;       LD  A, 18H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 01h         ;       LD  A, 01H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 1Dh         ;       LD  A, 1DH
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    3Eh, 02h         ;       LD  A, 02H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 00h         ;       LD  A, 00H
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    3Eh, 03h         ;       LD  A, 03H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh,0C1h         ;       LD  A, C1H
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    3Eh, 04h         ;       LD  A, 04H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 44h         ;       LD  A, 44H
DB   0EDh, 79h         ;       OUT (C), A
DB    3Eh, 05h         ;       LD  A, 05H
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    3Eh, 68h         ;       LD  A, 68H
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB   0FBh              ;       EI
DB     0,0,0
;DB    0Eh, 1Eh         ;       LD  C, 1EH
;DB    3Eh, 01h         ;       LD  A, 01H  ;off-hook
;DB   0EDh, 79h         ;       OUT (C), A
;DB  76H ;!!!!!!!!!! HALT                ;
DB   0D9h              ;       EXX           ; 4t
DB   0C3h
DW   NextStep          ;       JP NextStep
