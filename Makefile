BDIR=build

all: flash_alternator

flash_alternator: alternator.ihx
	${HOME}/.local/bin/stm8flash -c stlinkv2 -p stm8s003?3 -s flash -w $(BDIR)/$<.ihx

$(BDIR)/alternator.ihx: alternator.c
	sdcc -mstm8 $^
