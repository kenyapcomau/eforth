;   ?RX         ( -- c T | F )
;               Return input character and true, or a false if no input.

                $CODE   3,'?RX',QRX
DB    21h, 14h,0FEh    ;       LD  HL, SiobRxQout  ;16t
DB    7Eh              ;       LD  A, (HL)         ; 7t
DB    21h, 12h,0FEh    ;       LD  HL, SiobRxQin   ;16t
DB   0BEh              ;       CP  (HL)            ; 7t
DB    20h, 07h         ;       JR  NZ,$1           ; 7t/12t (NZ jump=12t) ptr <>, get the char
DB    21h, 00h, 00h    ;       LD  HL, 0           ;10t   False flag
DB   0E5h              ;       PUSH HL             ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep        ;10t ==> False Timing:84t==(10MHz)8.4usec
                       ;                           ;
DB    5Eh              ;$1:    LD  E, (HL)         ; 7t  get this ptr LOW
DB    23h              ;       INC HL              ; 6t
DB    56h              ;       LD  D, (HL)         ; 7t  get this ptr HI
DB   0EBh              ;       EX  DE,HL           ; 4t  let HL point the char
DB    5Eh              ;       LD  E, (HL)         ; 7t  get the char
DB    16h, 00h         ;       LD  D, 0            ; 7t  high byte =0
DB   0D5h              ;       PUSH DE             ;11t
DB    2Ch              ;       INC L               ; 4t  ptr+1, a 256 byte Ring queue
DB    22h, 12h,0FEh    ;       LD  (SiobRxQin),HL  ;16t
DB    21h,0FFh,0FFh    ;       LD  HL,0FFFFH       ;10t
DB   0E5h              ;       PUSH HL             ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep        ;10t ==> TRUE case: 100t(10MHz)10.0usec

;
;   TX!         ( c -- )
;               Send character c to the output device.

                $COLON  3,'TX!',TXSTO
TX1             DW      DOLIT, 0, DOLIT, 01BH, PCSTO ; BEGIN 0 $1B PC!
                DW      DOLIT, 1BH, PCAT             ;         $1B PC@
                DW      DOLIT, 4, ANDD               ;       4 AND
                DW      QBRAN,TX1                     ; UNTIL
                DW      DOLIT, 01AH, PCSTO, EXIT     ; $1A PC! ;




SiobRxBAvl:
       ; for timing: interrupt will get vector, push pc, jump to service .. .
       ;  a rough estimate:           7t   +       11t  +  10t
DB  0F5h               ;       PUSH AF             ;11t ;save
DB  0E5h               ;       PUSH HL             ;11t ;save
DB  0DBh, 1Ah          ;       IN  A, (1AH)        ;11t ;SIOB data reg
DB   2Ah, 14h,0FEh     ;       LD  HL, (SiobRxQout);16t ;
DB   77h               ;       LD  (HL), A         ; 7t ; put to ring queue
DB   2Ch               ;       INC L               ; 4t ; 256 byte ring queue
DB   22h, 14h,0FEh     ;       LD (SiobRxQout),HL  ;16t ;restore
DB  0E1h               ;       POP HL              ;10t ;restore
DB  0F1h               ;       POP AF              ;10t ;restore
DB  0FBh               ;       EI                  ; 4t
DB  0EDh, 4Dh          ;       RETI                ;14t
                                    ;114t + (entering 28t) => 142t==(10MHz)14.2usec


SiobSRxCond:      ; SIOB Special Rx Condition, reset only
DB  0F5h               ;       PUSH AF    ; NOTE: a flag may be set for discard a process
DB  0C5h               ;       PUSH BC
DB   3Eh, 04h          ;       LD  A, 4
DB   0Eh, 1Bh          ;       LD  C, 1BH
DB  0EDh, 79h          ;       OUT (C), A  ; reset
DB   3Eh,0FBh          ;       LD  A, 0FBH
DB   0Eh, 1Ch          ;       LD  C, 01CH
DB  0EDh, 79h          ;       OUT (C), A
DB  0C1h               ;       POP BC
DB  0F1h               ;       POP AF
DB  0FBh               ;       EI
DB  0EDh, 4Dh          ;       RETI
