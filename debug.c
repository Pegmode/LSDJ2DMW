#include "debug.h"


void decompressTest(){
    uint8_t lsdjFile[0x8000] = {0};    
    int fileIndex = 2;
    decompressLSDJFile(fileIndex,"test1.sav",lsdjFile);    

    //write test
    FILE* fo = fopen("lsdjFileOut.bin","wb");
    fwrite(lsdjFile,0x8000,1,fo);
    fclose(fo);
}


void test(){
    //decompressTest();
    ripAllWavetables("test1.sav");
    //uint8_t lsdjFile[0x8000];
    //decompressLSDJFile(2,"test1.sav",lsdjFile);
    printAllFilenames("test1.sav");
    /*
    uint8_t** wavetables;
    int wtLength = 3;
    wavetables = allocWavetableArray(wtLength);

    uint8_t lsdjFile[0x8000] = {0};    
    int fileIndex = 0;
    decompressLSDJFile(fileIndex,"test1.sav",lsdjFile);    

    readWaveFromIndexLSDJ(lsdjFile,0,wavetables[1]);

    //memcpy(wavetables[1],wavebuffer,32);
    //printWave(wavebuffer);
    printWave(wavetables[0]);
    printWave(wavetables[1]);
    free(wavetables);
    */
}