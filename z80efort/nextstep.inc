;;   NextStep
;The Forth Inner Interpreter--IP (=BC)is pointing the To-Be-Exec one
;NextStep: ;The Forth Inner Interpreter--IP (=BC)is pointing the To-Be-Exec one

DB 0Ah		;LD  A, (BC); 7t
DB 03h		;INC BC     ; 6t
DB 6Fh		;LD  L, A   ; 4t
DB 0Ah		;LD  A, (BC); 7t
DB 03h		;INC BC     ; 6t
DB 67h		;LD  H, A   ; 4t
DB 0E9h		;JP  (HL)   ; 4t
; 38t==(10MHz)3.8 usec.
