#
#	Rules to rebuild eForth binary image
#
efz80.BIN:	efz80.z80 coldstart.inc initsio.inc kernel.inc \
		memory.inc nextstep.inc portio.inc siotxrx.inc sva.inc
	jwasm -Fl -bin efz80.z80
