#include "LSDJ2DMW.h"

/*
instrument names (1e7a-1fb9)
    names are 5 bytes long


pay attention to wave instrument param length!!!
    indexed from 0
*/



int main(int argc, char* argv[]){
    if(argc <= 1){
        printf(HELPSTRING);
        exit(0);
    }
    if(strcmp("-h",argv[1]) == 0){
        printf(HELPSTRING);
        exit(0);
    }
    if(strcmp(argv[1],"") == 0){
        printf("ERROR: input filename not entered\n");
        printf(HELPSTRING);
        exit(1);
    }
    if(access(argv[1], F_OK ) == -1){
        printf("ERROR: file '%s' does not exist\n",argv[1]);
        printf(HELPSTRING);
        exit(1);        
    }
    strcpy(GLOBALINFILE,argv[1]);
    int runcode = 0;
    for(int i = 2; i < argc; i++){
        if(strcmp("-p",argv[i]) == 0){//print ripped waves and all songs
            GLOBALWAVEPRINTFLAG = 1;
            GLOBALFILEPRINTFLAG = 1;
        }
        else if(strcmp("-pw",argv[i]) == 0){//print ripped waves
            GLOBALWAVEPRINTFLAG = 1;
        }
        else if(strcmp("-pf",argv[i]) == 0){//print all songs
            GLOBALFILEPRINTFLAG =  1;
        }
 
        else if(strcmp("-l",argv[i]) == 0){//rip all wavetables in loaded song
            runcode = 5;
        }
        else if(strcmp("-lw",argv[i]) == 0){//rip single wave in loaded song
            GLOBALRIPWAVENUM = atoi(argv[++i]);
            runcode = 6;
        }        
        else if(strcmp("-lwr",argv[i]) == 0){//rip range of wavetables from loaded song
            GLOBALRIPWAVENUM = atoi(argv[++i]);
            GLOBALENDWAVENUM = atoi(argv[++i]);
            if (GLOBALRIPWAVENUM>GLOBALENDWAVENUM){
                printf("ERROR: first value in range cannot be larger than the second value in range\n");
                exit(1);
            }
            runcode = 7;
        }

       else if(strcmp("-a",argv[i]) == 0){//rip all waves in all songs
            runcode = 4; 
        }
        else if(strcmp("-s",argv[i]) == 0){//rip single song
            strcpy(GLOBALSONGNAME,argv[++i]);
            runcode = 1;
        }
        else if(strcmp("-sw",argv[i]) == 0){//rip single wave in song
            strcpy(GLOBALSONGNAME,argv[++i]);
            GLOBALRIPWAVENUM = atoi(argv[++i]);
            //GLOBALRIPWAVENUM = argv[++i];
            runcode = 2;
        }
        else if(strcmp("-swr",argv[i]) == 0){//rip range of waves in song
            strcpy(GLOBALSONGNAME,argv[++i]);
            GLOBALRIPWAVENUM = atoi(argv[++i]);
            GLOBALENDWAVENUM = atoi(argv[++i]);
            if (GLOBALRIPWAVENUM>GLOBALENDWAVENUM){
                printf("ERROR: first value in range cannot be larger than the second value in range\n");
                exit(1);
            }
            //GLOBALRIPWAVENUM = argv[++i];
            //GLOBALENDWAVENUM = argv[++i];
            runcode = 3;   
        }
        else if(strcmp("-x",argv[i]) == 0){
            GLOBALOUTPUTINDEXMODE = 1;
        }
        else if(strcmp("-nw",argv[i]) == 0){
            runcode = 10;  
        }      
        else if(strcmp("-h",argv[i]) == 0){
            printf(HELPSTRING);
            exit(0);
        }
        else if(strcmp("-d",argv[i]) == 0){
            test();
            exit(0);
        }
    }
    switch (runcode){
    case 0:// rip all wavetables in loaded song(default)
        ripWaveRangeFromLoaded(GLOBALINFILE,0,255);
        break;
    case 1://single file
        ripSingleSong(GLOBALINFILE,GLOBALSONGNAME);
        break;
    case 2://single wavetable in single song
        ripWavetableRangeFromSong(GLOBALINFILE,GLOBALSONGNAME,GLOBALRIPWAVENUM,GLOBALRIPWAVENUM);
        break;
    case 3://rip range of wavetables in single song
        ripWavetableRangeFromSong(GLOBALINFILE,GLOBALSONGNAME,GLOBALRIPWAVENUM,GLOBALENDWAVENUM);
        break;
    case 4://rip all in all songs
        ripAllWavetables(GLOBALINFILE);
        break;
    case 5://rip all wavetables in loaded song
        ripWaveRangeFromLoaded(GLOBALINFILE,0,255);
        break;
    case 6://rip single wavetable in loaded song
        ripWaveRangeFromLoaded(GLOBALINFILE,GLOBALRIPWAVENUM,GLOBALRIPWAVENUM);
        break;
    case 7://rip wavetables in given range of song
        ripWaveRangeFromLoaded(GLOBALINFILE,GLOBALRIPWAVENUM,GLOBALENDWAVENUM);
        break;
    case 10://do nothing -nw
        break;
    default://something went wrong
        break;
    }
    if(GLOBALFILEPRINTFLAG){
        printAllFilenames("test1.sav");
    }
}

