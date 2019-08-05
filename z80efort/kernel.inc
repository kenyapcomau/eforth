;; The kernel

;   doLIT       ( -- w )
;               Push an inline literal.

                $CODE   COMPO+5,'doLIT',DOLIT
DB    0Ah              ;       LD  A, (BC)  ; 7t
DB    03h              ;       INC BC       ; 6t
DB    6Fh              ;       LD  L,A      ; 4t
DB    0Ah              ;       LD  A,(BC)   ; 7t
DB    03h              ;       INC BC       ; 6t
DB    67h              ;       LD  H,A      ; 4t
DB   0E5h              ;       PUSH HL      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t    ;(10MHz)5.5usec (+2.5usec NextStep)


;   doLIST      ( a -- )
;               Process colon list.

                $CODE   COMPO+6,'doLIST',DOLST
DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP) ;16t
DB    2Bh              ;       DEC HL       ; 6t
DB    70h              ;       LD  (HL), B  ; 7t      \ end of r-push simulation
DB    2Bh              ;       DEC HL       ; 6t
DB    71h              ;       LD  (HL), C  ; 7t      \ end of r-push simulation
DB    22h, 0Ch,0FEh    ;       LD  (RP), HL ;16t      \ IP is r-pushed (simulated)
               ;                    ;
DB   0C1h              ;       POP BC       ;10t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                      ;99t==9.9usec (+ 2.4usec for NextStep)



;   EXIT        ( -- )
;               Terminate a colon definition.

                $CODE   4,'EXIT',EXIT

DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP) ;16t
DB    4Eh              ;       LD  C, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    46h              ;       LD  B, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    22h, 0Ch,0FEh    ;       LD  (RP),HL  ;16t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;68t==6.8us



;   EXECUTE     ( ca -- )
;               Execute the word at ca.

                $CODE   7,'EXECUTE',EXECU
DB   0E1h              ;       POP HL        ;10t
DB   0E9h              ;       JP  (HL)      ; 4t



;   next        ( -- )
;               Run time code for the single index loop.
;               : next ( -- ) \ hilevel model
;                 r> r> dup if 1 - >r @ >r exit then drop cell+ >r ;

                $CODE   COMPO+4,'next',DONXT
DB   2Ah, 0Ch,0FEh     ;       LD  HL, (RP) ;16t
DB   7Eh               ;       LD  A, (HL)  ; 7t
DB  0B7h               ;       OR  A        ; 4t
DB   20h, 0Eh          ;       JR  NZ, DECLOW;12/7t a fast dec is ok, only failed every 255 time
                       ;                    ; low byte 0
DB   23h               ;       INC HL       ; 6t
DB   7Eh               ;       LD  A, (HL)  ; 7t
DB  0B7h               ;       OR  A        ; 4t
DB   20h, 0Ch          ;       JR  NZ, DECHILO;12/7t Hi-byte no-zero, it is also a re-loop case
                       ;zero bound now .. .
DB   23h               ;       INC HL       ; 6tdiscard the loop count on R-stack
DB   22h, 0Ch,0FEh     ;       LD  (RP), HL ;16t
DB   03h               ;       INC BC       ; 6t\ IP slip over the re-loop-addr
DB   03h               ;       INC BC       ; 6t
DB  0C3h
DW  NextStep           ;       JP  NextStep ;10t loop is over
                       ;                    ; 98t==(10MHz)9.8usec
DB   35h              ;DECHILO:DEC (HL)     ;11t hi-byte
DB   2Bh               ;       DEC HL       ; 6t back to low byte
DB   35h               ;DECLOW:DEC (HL)     ;11t low byte non-zero, just dec it and re-loop
DB   69h               ;       LD  L, C     ; 4t get loop-start-adr to IP and keep stepping
DB   60h               ;       LD  H, B     ; 4t
DB   4Eh               ;       LD  C, (HL)  ; 7t
DB   23h               ;       INC HL       ; 6t
DB   46h               ;       LD  B, (HL)  ; 7t
DB  0C3h
DW  NextStep           ;       JP  NextStep ;10t
                              ; low byte dec:    88t==(10MHz)8.8usec
                              ; lo&Hi byte dec: 134t==(10MHz)13.4usec


;   ?brAhnch     ( f -- )
;               Branch if flag is zero.

                $CODE   COMPO+7,'?branch',QBRAN
DB   0E1h              ;       POP HL        ;10t
DB    7Dh              ;       LD  A, L      ; 4t    ?branch adr is just after ?branch
DB   0B4h              ;       OR  H         ; 4t    and IP is pointing it
DB    28h, 05h         ;       JR  Z,ZEROO   ; 12/7t (Z=1,12t)
DB    03h              ;       INC BC        ; 6t    IP slip over the retun addr
DB    03h              ;       INC BC        ; 6t    ex: 'TRUE IF' will slip over
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t  ;47t==(10MHz)4.7usec
                       ;
DB    69h              ;ZEROO: LD  L, C      ; 4t    get IP :=[IP] and go
DB    60h              ;       LD  H, B      ; 4t
DB    4Eh              ;       LD  C, (HL)   ; 7t
DB    23h              ;       INC HL        ; 6t
DB    46h              ;       LD  B, (HL)   ; 7t
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t  68t==(10MHz)6.8usec



;   branch      ( -- )
;               Branch to an inline address.

                $CODE   COMPO+6,'branch',BRAN
DB    69h              ;       LD  L, C      ; 4t   get IP :=[IP] and go
DB    60h              ;       LD  H, B      ; 4t
DB    4Eh              ;       LD  C, (HL)   ; 7t
DB    23h              ;       INC HL        ; 6t
DB    46h              ;       LD  B, (HL)   ; 7t
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t
                       ;                     ;38t==(10MHz)3.8usec



;   !           ( w a -- )
;               Pop the data stack to memory.

                $CODE   1,'!',STORE
DB   0E1h              ;       POP HL       ;10t
DB   0D1h              ;       POP DE       ;10t
DB    73h              ;       LD  (HL), E  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    72h              ;       LD  (HL), D  ; 7t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;50t==(10Mhz)5.0 usec



;   @           ( a -- w )
;               Push memory location to the data stack.

                $CODE   1,'@',AT
DB   0E1h              ;       POP HL       ;10t
DB    5Eh              ;       LD  E, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    56h              ;       LD  D, (HL)  ; 7t
DB   0D5h              ;       PUSH DE      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;51t==(10Mhz)5.1usec



;   C!          ( c b -- )
;               Pop the data stack to byte memory.

                $CODE   2,'C!',CSTOR
DB   0E1h              ;       POP HL        ;10t
DB   0D1h              ;       POP DE        ;10t
DB    73h              ;       LD  (HL), E   ; 7t
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t
                       ;                     ;37t==(10Mhz)3.7usec




;   C@          ( b -- c )
;               Push byte memory location to the data stack.

                $CODE   2,'C@',CAT
DB   0E1h              ;       POP HL       ;10t
DB    5Eh              ;       LD  E, (HL)  ; 7t
DB    16h, 00h         ;       LD  D, 0     ; 7t
DB   0D5h              ;       PUSH DE      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;45t==(10Mhz)4.5usec




;   >R          ( w -- )
;               Push the data stack to the return stack.

                $CODE   COMPO+2,'>R',TOR
DB   0D1h              ;       POP DE       ;10t
DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP) ;16t
DB    2Bh              ;       DEC HL       ; 6t
DB    72h              ;       LD  (HL), D  ; 7t
DB    2Bh              ;       DEC HL       ; 6t
DB    73h              ;       LD  (HL), E  ; 7t
DB    22h, 0Ch,0FEh    ;       LD  (RP), HL ;16t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;78t==(10MHz)7.8usec



;   R@          ( -- w )
;               Copy top of return stack to the data stack.

                $CODE   2,'R@',RAT
DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP) ;16t
DB    5Eh              ;       LD  E, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    56h              ;       LD  D, (HL)  ; 7t
DB   0D5h              ;       PUSH DE      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;57t==(10MHz)5.7usec



;   R>          ( -- w )
;               Pop the return stack to the data stack.

                $CODE   2,'R>',RFROM
DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP) ;16t
DB    5Eh              ;       LD  E, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    56h              ;       LD  D, (HL)  ; 7t
DB    23h              ;       INC HL       ; 6t
DB    22h, 0Ch,0FEh    ;       LD  (RP), HL ;16t
DB   0D5h              ;       PUSH DE      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;67t==(10MHz)5.7.usec



;   RP@         ( -- a )
;               Push the current RP to the data stack.

                $CODE   3,'RP@',RPAT
DB    2Ah, 0Ch,0FEh    ;       LD  HL, (RP)
DB   0E5h              ;       PUSH HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   RP!         ( a -- )
;               Set the return stack pointer.

                $CODE   COMPO+3,'RP!',RPSTO
DB   0E1h              ;       POP HL       ;
DB    22h, 0Ch,0FEh    ;       LD  (RP), HL ;
DB   0C3h
DW   NextStep          ;       JP  NextStep ;



;   SP@         ( -- a )
;               Push the current data stack pointer.

                $CODE   3,'SP@',SPAT
DB    21h, 00h, 00h    ;       LD  HL, 0
DB    39h              ;       ADD HL, SP
DB   0E5h              ;       PUSH HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   SP!         ( a -- )
;               Set the data stack pointer.

                $CODE   3,'SP!',SPSTO
DB   0E1h              ;       POP HL
DB   0F9h              ;       LD  SP, HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   DUP         ( w -- w w )
;               Duplicate the top stack item.

                $CODE   3,'DUP',DUPP
DB   0E1h              ;       POP HL       ;10t
DB   0E5h              ;       PUSH HL      ;11t
DB   0E5h              ;       PUSH HL      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;42t==(10MHz)4.2usec



;   DROP        ( w -- )
;               Discard top stack item.

                $CODE   4,'DROP',DROP
DB   0E1h              ;       POP HL       ;10t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;20t==(10MHz)2.0usec



;   SWAP        ( w1 w2 -- w2 w1 )
;               Exchange top two stack items.

                $CODE   4,'SWAP',SWAP
DB   0E1h              ;       POP HL       ;10t
DB   0E3h              ;       EX  (SP), HL ;19t
DB   0E5h              ;       PUSH HL      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;50t==(10MHz)5.0usec



;   OVER        ( w1 w2 -- w1 w2 w1 )
;               Copy second stack item to top.

                $CODE   4,'OVER',OVER
DB   0D1h              ;       POP DE       ;10t
DB   0E1h              ;       POP HL       ;10t
DB   0E5h              ;       PUSH HL      ;11t
DB   0D5h              ;       PUSH DE      ;11t
DB   0E5h              ;       PUSH HL      ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep ;10t
                       ;                    ;63t==(10MHz)6.3usec



;   0<          ( n -- t )
;               Return true if n is negative.

                $CODE   2,'0<',ZLESS
DB   0E1h              ;       POP HL         ;10t
DB    29h              ;       ADD HL, HL     ;11t
DB   038h, 07h         ;       JR  C, LESSZ   ;12/7t
DB    21h, 00h, 00h    ;       LD  HL, 0      ;10t
DB   0E5h              ;       PUSH HL        ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep   ;10t   59t==(10MHz)5.9usec
                       ;
DB    21h,0FFh,0FFh    ;LESSZ: LD  HL, 0FFFFH ;10t
DB   0E5h              ;       PUSH HL        ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep   ;10t   64t==(10MHz)6.4usec



;   AND         ( w w -- w )
;               Bitwise AND.

                $CODE   3,'AND',ANDD
DB   0D1h              ;       POP DE
DB   0E1h              ;       POP HL
DB    7Bh              ;       LD  A, E
DB   0A5h              ;       AND L
DB    6Fh              ;       LD  L, A
DB    7Ah              ;       LD  A, D
DB   0A4h              ;       AND H
DB    67h              ;       LD  H, A
DB   0E5h              ;       PUSH HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   OR          ( w w -- w )
;               Bitwise inclusive OR.

                $CODE   2,'OR',ORR
DB   0D1h              ;       POP DE
DB   0E1h              ;       POP HL
DB    7Bh              ;       LD  A, E
DB   0B5h              ;       OR  L
DB    6Fh              ;       LD  L, A
DB    7Ah              ;       LD  A, D
DB   0B4h              ;       OR  H
DB    67h              ;       LD  H, A
DB   0E5h              ;       PUSH HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   XOR         ( w w -- w )
;               Bitwise exclusive

                $CODE   3,'XOR',XORR
DB   0D1h              ;       POP DE
DB   0E1h              ;       POP HL
DB    7Bh              ;       LD  A, E
DB   0ADh              ;       XOR L
DB    6Fh              ;       LD  L, A
DB    7Ah              ;       LD  A, D
DB   0ACh              ;       XOR H
DB    67h              ;       LD  H, A
DB   0E5h              ;       PUSH HL
DB   0C3h
DW   NextStep          ;       JP  NextStep



;   UM+         ( w w -- w cy )
;               Add two numbers, return the sum and carry flag.

                $CODE   3,'UM+',UPLUS
DB   0D1h              ;       POP DE         ;10t
DB   0E1h              ;       POP HL         ;10t
DB    19h              ;       ADD HL, DE     ;11t
DB   0E5h              ;       PUSH HL        ;11t
DB    38h, 07h         ;       JR  C, CARRY   ;12/7t
DB    21h, 00h, 00h    ;       LD  HL, 0      ;10t
DB   0E5h              ;       PUSH HL        ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep   ;10t
                       ;
DB    21h, 01h, 00h    ;CARRY: LD  HL, 1      ;10t
DB   0E5h              ;       PUSH HL        ;11t
DB   0C3h
DW   NextStep          ;       JP  NextStep   ;10t



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


;   PC!         ( uc p --  )
;               output uc to IO port p
;
                $CODE   3,'PC!',PCSTO
DB   0D9h              ;       EXX           ; 4t
DB   0C1h              ;       POP BC        ;10t
DB   0D1h              ;       POP DE        ;10t
DB    7Bh              ;       LD  A, E      ; 4t
DB   0EDh, 79h         ;       OUT (C), A    ;12t
DB   0D9h              ;       EXX           ; 4t
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t
                       ;                     ;54t==(10MHz)5.4usec

;   PC@         ( p -- uc )
;               output uc to IO port p

                $CODE   3,'PC@',PCAT
DB   0D9h              ;       EXX           ; 4t
DB   0C1h              ;       POP BC        ;10t
DB   0EDh, 78h         ;       IN  A, (C)    ;12t
DB    5Fh              ;       LD  E, A      ; 4t
DB    16h, 00h         ;       LD  D, 0      ; 7t
DB   0D5h              ;       PUSH DE       ;11t
DB   0D9h              ;       EXX           ; 4t
DB   0C3h
DW   NextStep          ;       JP  NextStep  ;10t
                       ;                     ;62t==(10MHz)6.2usec


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
