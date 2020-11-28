C = gcc
outname = LSDJ2DMW

ifeq ($(OS),Windows_NT)
outname = $(outname).exe
systemos = 0
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	systemos = 1
endif
endif

dependencies = LSDJ2DMW.c LSDJ2DMW.h utils.c utils.h ripTools.c LSDJDecompress.c LSDJDecompress.h debug.c debug.h

$(outname): $(dependencies)
	$(C) LSDJ2DMW.c -o $(outname) -DSYSTEM_OS=$(systemos)

clean:
	rm -f $(outname)

.PHONY: clean
