#
#	Rules to rebuild eForth binary image
#

efz80.com:	efz80.bin
		tail -c +257 $< > $@

efz80.bin:	efz80.ihx
		srec_cat $< -intel -output $@ -binary

efz80.ihx:	efz80.z80 bye.z80 coldstart.z80 initdev.z80 kernel.z80 \
		lowmem.z80 nextstep.z80 portio.z80 siorxtx.z80 \
		uinit.z80 words.z80 system.def
		asz80 -l -o $< 2>${<:.z80=.err} && \
			aslink -i -o $(<:.z80=.rel)

clean:
		rm -f efz80.bin efz80.com efz80.err efz80.hlr efz80.ihx efz80.lst efz80.rel
