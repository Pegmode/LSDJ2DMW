#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#ifndef SYSTEMOS
    #define SYSTEMOS 0//windows
#endif
int GLOBALWAVEPRINTFLAG = 0;//print waves
int GLOBALFILEPRINTFLAG = 0;//print scanned files
int GLOBALOUTFILEFLAG = 1;//output .dmw's

#if SYSTEMOS == 0//windows
    char* GLOBALFILEDIRECTORY = ".\\outputwaves\\";//directory to dump files into  (windows)
#endif
#if SYSTEMOS == 1//linux
    char* GLOBALFILEDIRECTORY = "./outputwaves/";//directory to dump files into  (linux)
#endif
char GLOBALINFILE[50] = "";
char GLOBALSONGNAME[50] = "";
int GLOBALRIPWAVENUM = 0;//lazy solution
int GLOBALENDWAVENUM = 0;
int GLOBALOUTPUTINDEXMODE = 0;//1 = dec, 2 = hex
#define GBWAVESIZE 32
#define GBBITDEPTH 4
#define LSDJWAVESTARTADDRESS 0x6000
#define LSDJWAVEENDADDRESS 0x6FFF
#define LSDJVERSIONADDRESS 0x7FFF
#define FILENAMES_BASEADDRESS 0x8000
#define FILENAME_LENGTH 8

char* HELPSTRING = \
"==LSDJ2DMW 1.0==\n\
by Pegmode (Dan Chu)\n\n\
Rips wavetables from LSDJ into .dmw for use in Deflemask\n\
USAGE: LSDJ2SDMW <inputSavFilename> [options]\n\
OPTIONS: \n\
-l: rip all wavetables in loaded module (default)\n\
-lw <wavenumber>: rips a single wavetable from the loaded module\n\
-lwr <startWaveNumber>: rip a range of wavetables from the loaded module\n\
-a: rips all wavetables from all saved songs\n\
-s: <songname>: rip all wavetables from a single song\n\
-sw: <songname> <wavenumber>: rips a specific wavetable from a given song (0-255)\n\
-swr: <songname> <startWaveNumber> <endWaveNumber>: rips wavetables in a given range in a specific song (0-255)\n\
-p: print ripped waves and files\n\
-pw: print ripped waves\n\
-pf: print songs\n\
-x: format outputted wavetable indices as hex\n\
-nw: don't write files or parse files. Useful in combination with -p/-pf for reading songnames\n\
\n\
Example usage: LSDJ2SFMW test.sav -p -lwr 0 5\n\
default behavior rips all wavetables in loaded song \n\
for all behavior involving the 'loaded song', the 'loaded song' means what you have loaded in LSDj before saving\n\
by default waves are dumped into directory ./outputwaves/\n\
LSDJ versions less than 6.7.0 are not guaranteed to function properly with this program\n\
";

#include "LSDJDecompress.c"
#include "utils.c"
#include "ripTools.c"
#include "debug.c"



/*
Version Codes
 * 0: < 3.6.0
 * 1: 3.6.0
 * 2: 3.6.1
 * 3: 4.4.0
 * 4: 5.7.0
 * 5: 6.3.0
 * 6: 6.7.0
 * 7: 6.8.0
 * 8: 7.1.0
 * 9: 7.5.0
 * 10: 7.9.8
*/
