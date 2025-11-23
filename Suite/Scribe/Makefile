# Makefile - builds stage0, bootstraps all .op files and creates 'scribe' wrapper.
CC = gcc
STAGE0 = stage0
FLUXDIR = flux
TOOLS = tools
VM = $(TOOLS)/flux_vm

FLUX_SOURCES := $(wildcard $(FLUXDIR)/*.flux)
OP_FILES := $(patsubst $(FLUXDIR)/%.flux,$(FLUXDIR)/%.op,$(FLUX_SOURCES))

.PHONY: all clean bootstrap ops install

all: bootstrap ops

bootstrap: $(STAGE0)

$(STAGE0): src/stage0.c
	$(CC) -O2 $< -o $@

# generate .op for each .flux with stage0
$(FLUXDIR)/%.op: $(FLUXDIR)/%.flux $(STAGE0)
	./$(STAGE0) $< $@

ops: $(OP_FILES)

# create a tiny wrapper script 'scribe' that runs flux_vm with scribe_main.op
install: all
	@echo "#!/bin/sh" > scribe
	@echo 'DIR="$(cd "$(dirname "$0")" && pwd)"' >> scribe
	@echo 'exec "$(pwd)/$(VM)" "$(pwd)/$(FLUXDIR)/scribe_main.op" "$@"' >> scribe
	chmod +x scribe
	@echo "Installed ./scribe - run './scribe <input.flux> <out.op>'"

clean:
	rm -f $(STAGE0) $(FLUXDIR)/*.op scribe
