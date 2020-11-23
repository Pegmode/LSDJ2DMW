C = gcc
outname = LSDJ2DMW
dependencies = LSDJ2DMW.c LSDJ2DMW.h utils.c utils.h ripTools.c ripTools.h LSDJDecompress.c LSDJDecompress.h debug.c debug.h
windows: $(dependencies) 
	$(C) LSDJ2DMW.c -o $(outname).exe
linux: $(dependencies) #untested
	$(C) LSDJ2DMW.c -o $(outname) -D SYSTEMOS = 1