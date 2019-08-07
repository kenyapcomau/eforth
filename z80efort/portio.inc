;   PC!         ( uc p --  )
;               output uc to IO port p

                $CODE   3,'PC!',PCSTO
DB 0D9h		;EXX           ; 4t
DB 0C1h		;POP BC        ;10t
DB 0D1h		;POP DE        ;10t
DB 7Bh		;LD  A, E      ; 4t
DB 0EDh,79h	;OUT (C), A    ;12t
DB 0D9h		;EXX           ; 4t
DB 0C3h
DW NextStep          ;       JP  NextStep  ;10t
                       ;                     ;54t==(10MHz)5.4usec

;   PC@         ( p -- uc )
;               input uc from IO port p

                $CODE   3,'PC@',PCAT
DB 0D9h		;EXX           ; 4t
DB 0C1h		;POP BC        ;10t
DB 0EDh,78h	;IN  A, (C)    ;12t
DB 5Fh		;LD  E, A      ; 4t
DB 16h,00h	;LD  D, 0      ; 7t
DB 0D5h		;PUSH DE       ;11t
DB 0D9h		;EXX           ; 4t
DB 0C3h
DW NextStep          ;       JP  NextStep  ;10t
                       ;                     ;62t==(10MHz)6.2usec
