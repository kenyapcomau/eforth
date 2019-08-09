#
#	Rules to rebuild eForth binary image
#

efz80.bin:	efz80.ihx
		srec_cat $< -intel -output $@ -binary

efz80.ihx:	efz80.z80 coldstart.z80 initdev.z80 kernel.z80 \
		lowmem.z80 nextstep.z80 portio.z80 siorxtx.z80 \
		words.z80 system.def
		asz80 -l -o $< 2>${<:.z80=.err} && \
			aslink -i -o $(<:.z80=.rel)

#	Compare binary output with original

compare:	../z80efort/EFZ80.COM efz80.bin
		@echo 'Should only differ in 8 padding bytes, 0 instead of 220'
		@echo ' and these bytes'
		@echo ' 389  16  20'
		@echo ' 394  14  11'
		-cmp -l ../z80efort/EFZ80.COM efz80.bin

clean:
		rm -f efz80.bin efz80.err efz80.hlr efz80.ihx efz80.lst efz80.rel
