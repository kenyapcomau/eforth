#
#	Rules to rebuild eForth binary image
#

efz80.BIN:	efz80.z80 coldstart.inc initdev.inc kernel.inc \
		lowmem.inc memory.inc nextstep.inc portio.inc siorxtx.inc \
		words.inc
		jwasm -Fl -bin efz80.z80

coldstart.inc initdev.inc kernel.inc lowmem.inc nextstep.inc portio.inc siorxtx.inc:	system.def

#	Rule to make an .inc file from .z80 source

%.inc:		%.z80 lsttoinc.py
		asz80 -lpu $< && ./lsttoinc.py < $(<:.z80=.lst) > $(<:.z80=.inc)
		rm -f $(<:.z80=.lst)

#	Compare binary output with original

compare:	EFZ80.COM efz80.BIN
		@echo 'Should only differ in 8 padding bytes, 0 instead of 220'
		@echo ' and these bytes'
		@echo ' 389  16  20'
		@echo ' 394  14  11'
		-cmp -l EFZ80.COM efz80.BIN
