ORG            BM + 100H


DB   0F3h              ;       DI
                       ;
                       ; move ROM version user data to 1st user RAM
                       ;
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
                       ;
DB    0Eh, 1Ch         ;       LD  C, 1CH
DB    3Eh, 5Bh         ;       LD  A, 05BH     ;PIOA data: TEST ON, RED LED on .. .
DB   0EDh, 79h         ;       OUT (C), A
                       ;
DB    0Eh, 1Eh         ;       LD  C, 1EH
DB    3Eh, 17h         ;       LD  A, 17H     ;PIOB data: .. .
DB   0EDh, 79h         ;       OUT (C), A
                       ;
                       ; Set IM2
DB   0EDh, 5Eh         ;       IM 2
                       ;
                       ; init SP, RP, UP
DB    21h,0FEh,0E1h    ;       LD  HL, SPP
DB   0F9h              ;       LD  SP, HL    ; INIT SP
                       ;
DB    21h,0FEh,0E0h    ;       LD   HL, RPP   ;   init to RPP
DB    22h, 0Ch,0FEh    ;       LD   (RP), HL  ;   simulated R-stack reg at RP
                       ;
DB    21h, 00h,0E1h    ;       LD   HL, UPP    ; use the initial USER VARIABLE area
DB    22h, 0Eh,0FEh    ;       LD   (UP), HL   ; simulated R-stack reg at RP
                       ;
DB    01h, 00h, 00h    ;       LD  BC, ERROR   ;jmp 0 cold-start whenever R-stack underflow-used
                       ;                 ; this is supposed not impossible to be happen

DB    0Eh, 1Ch         ;       LD  C, 1CH
DB    3Eh,0BBh         ;       LD  A, 0BBH     ;PIOA data: GRN LED only  .
DB   0EDh, 79h         ;       OUT (C), A

DB   0C3h              ;       JP  COLD
DW   COLD
