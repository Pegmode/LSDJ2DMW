#include "ripTools.h"

void writeRangeFromDecompressedBuffer(int startWaveNum, int endWaveNum, char*  lsdjFileName, uint8_t* lsdjFile){//start wave and end wave cannot be out of range 0-255
    char outname[50];
    uint8_t wavetableBuffer[32];
    uint8_t defaultWave4B[] = {0x8,0xe,0xc,0xd,0xc,0xc,0xb,0xb,0xa,0xa,0xa,0x9,0x9,0x9,0x8,0x8,0x8,0x7,0x7,0x6,0x6,0x6,0x5,0x5,0x5,0x4,0x4,0x3,0x3,0x2,0x3,0x1};
    if((endWaveNum > 255 ) | (endWaveNum < 0)){
        printf("ERROR: wave number '%i' is out of range 0-255\n",endWaveNum);
        exit(1);
    }
    if((startWaveNum > 255 ) | (startWaveNum< 0)){
        printf("ERROR: wave number '%i' is out of range 0-255 (startwave)\n",endWaveNum);
        exit(1);
    }
    if (startWaveNum > endWaveNum){
        printf("ERROR: wrong write ranges \n");
        exit(1);
    }
    for(int j = startWaveNum; j < endWaveNum+1; j++){//range is from 0 to 255
        readWaveFromIndexLSDJ(lsdjFile,j,wavetableBuffer);
        if(memcmp(wavetableBuffer,defaultWave4B,32) != 0){
            if(GLOBALWAVEPRINTFLAG){//change this to a debug print call
            printf("%s wave %d: ",lsdjFileName,j);
            printWave(wavetableBuffer);
            }
            //build filestring
            if(GLOBALOUTPUTINDEXMODE){
               sprintf(outname,"%s%s_Wave%X",GLOBALFILEDIRECTORY,lsdjFileName,j); 
            }
            else{
                sprintf(outname,"%s%s_Wave%d",GLOBALFILEDIRECTORY,lsdjFileName,j);
            }
            writeDMW(wavetableBuffer,outname);
        }
    }
}

void ripAllWavetables(char* filepath){
    uint8_t lsdjFile[0x8000];
    uint8_t wavetableBuffer[32];
    char lsdjFileName[9];
    for(int i = 0; i < 20; i++){
        if(findFirstBlockFromFileIndex(i,filepath) != -1){//does file index exist?
            findNameFromFileindex(filepath,lsdjFileName,i);
            decompressLSDJFile(i,filepath,lsdjFile);
            writeRangeFromDecompressedBuffer(0,255,lsdjFileName,lsdjFile);
        }
    }
}


void ripSingleSong(char* filepath,char* songName){
    uint8_t lsdjFile[0x8000];
    uint8_t wavetableBuffer[32];
    //char lsdjFileName[9];
    int fileIndex = findFileindexFromName(filepath,songName);
    if(findFirstBlockFromFileIndex(fileIndex,filepath) != -1){//does file index exist?
        decompressLSDJFile(fileIndex,filepath,lsdjFile);
        writeRangeFromDecompressedBuffer(0,255,songName,lsdjFile);
    }
    else{
        printf("ERROR: file '%s' not found",songName);
    }

}

void ripWavetableRangeFromSong(char* filepath,char* songName,int startWavetableNumber,int endWavetableNumber){
    uint8_t lsdjFile[0x8000];
    uint8_t wavetableBuffer[32];
    //char lsdjFileName[9];
    int fileIndex = findFileindexFromName(filepath,songName);
    if(findFirstBlockFromFileIndex(fileIndex,filepath) != -1){//does file index exist?
        decompressLSDJFile(fileIndex,filepath,lsdjFile);
        writeRangeFromDecompressedBuffer(startWavetableNumber,endWavetableNumber,songName,lsdjFile);
    }
    else{
        printf("ERROR: file '%s' not found",songName);
    }

}

void ripWaveRangeFromLoaded(char* filepath,int startWavetableNumber, int endWavetableNumber){//loads range from active project
    uint8_t lsdjFile[0x8000];
    uint8_t wavetableBuffer[32];
    char songName[9];
    FILE* f = fopen(filepath,"rb");
    fread(lsdjFile,0x8000,1,f);
    fclose(f);
    int loadedIndex = getLoadedSongindex(filepath);
    findNameFromFileindex(filepath, songName, loadedIndex);
    writeRangeFromDecompressedBuffer(startWavetableNumber,endWavetableNumber,songName,lsdjFile);
}
