BDIR=build
BDIRA=$(BDIR)/alternator
SDCC=sdcc -mstm8

all: init_dir flash_alternator

init_dir:
	@mkdir -p $(BDIRA)

flash_alternator: $(BDIRA)/main.ihx
	${HOME}/.local/bin/stm8flash -c stlinkv2 -p stm8s003?3 -s flash -w $<

$(BDIRA)/main.ihx: $(BDIRA)/main.rel $(BDIRA)/isr_handlers.rel $(BDIRA)/utils.rel $(BDIRA)/init.rel
	$(SDCC) -o $@ $^

$(BDIRA)/main.rel: alternator/main.c defines.h isr_handlers.h regmap.h alternator/utils.h alternator/init.h
	$(SDCC) --compile-only -o $@ $<

$(BDIRA)/isr_handlers.rel: alternator/isr_handlers.c isr_handlers.h regmap.h alternator/utils.h 
	$(SDCC) --compile-only -o $@ $<

$(BDIRA)/utils.rel: alternator/utils.c alternator/utils.h regmap.h
	$(SDCC) --compile-only -o $@ $<

$(BDIRA)/init.rel: alternator/init.c alternator/init.h regmap.h defines.h
	$(SDCC) --compile-only -o $@ $<

clean:
	rm -vrf build/*
