BDIR=build

all: $(BDIR)/alternator.ihx

flash_alternator: $(BDIR)/alternator.ihx
	${HOME}/.local/bin/stm8flash -c stlinkv2 -p stm8s003?3 -s flash -w $(BDIR)/$<.ihx

$(BDIR)/alternator.ihx: alternator.c
	sdcc -mstm8 -o $@ $^

alternator.c : defines.h regmap.h

clean:
	rm -vrf build/*
