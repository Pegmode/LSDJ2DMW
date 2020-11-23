#include "utils.h"

void printWave(uint8_t* wave){
    printf("Wave: ");
    for(int i = 0; i < GBWAVESIZE; i++){
        printf("%X ",wave[i]);
    }
    printf("\n");
}

uint8_t getLSDJVersion(char* filepath){
    FILE* f = fopen(filepath, "rb");
    uint8_t version;
    fseek(f,LSDJVERSIONADDRESS,SEEK_SET);
    fread(&version,1,sizeof(uint8_t),f);
    fclose(f);
    return version;
}

void wavetableTo4bit(uint8_t* inputWave,uint8_t* outputWave){//pad from 8 bit to 4 bit
    for(int i = 0; i < 16;i++){
        //low nibble
            outputWave[i*2+1] = 0xF&inputWave[i];
        //high nibble
            outputWave[i*2] = (0xF0&inputWave[i])>>4;
    }
}

int findFileindexFromName(char* filepath, char* filename){
    //Files are indexed from 0
    char songNames[256];//
    FILE* f = fopen(filepath,"rb");
    fseek(f,FILENAMES_BASEADDRESS,SEEK_SET);
    fread(songNames,256,1,f);
    fclose(f);
    for(int i = 0; i < 20; i++){
        if(memcmp(filename,&songNames[i*8],8) == 0){
            return i;
        }
    }
    return -1;
}

void findNameFromFileindex(char* filepath, char* filename, int index){
    //filename string is 9 long w/ null term
    //Files are indexed from 0
    char songNames[256];//
    FILE* f = fopen(filepath,"rb");
    fseek(f,FILENAMES_BASEADDRESS,SEEK_SET);
    fread(songNames,256,1,f);
    fclose(f);

    memcpy(filename,&songNames[8*index],8);
    songNames[9] = '\0';

}

void readWaveFromIndexFILERAW(char* fileName,int waveNumber,uint8_t* wave){//reads a single wave 
    if(waveNumber >= 256){
        printf("Warning: waveNumber %d is out of the indexable range (0-255)\n",waveNumber);
        return;
    }
    uint8_t waveBuffer[GBWAVESIZE/2]; 
    int waveOffset = (waveNumber*16)+LSDJWAVESTARTADDRESS;
    FILE* f = fopen(fileName,"rb");
    fseek(f,waveOffset,SEEK_SET);
    fread(waveBuffer,GBWAVESIZE/2,sizeof(uint8_t),f);
    fclose(f);
    wavetableTo4bit(waveBuffer,wave);

}

void readWaveFromIndexLSDJ(uint8_t* lsdjFile, int waveNumber, uint8_t* wave){
    if(waveNumber >= 256){
        printf("Warning: waveNumber %d is out of the indexable range (0-255)\n",waveNumber);
        return;
    }
    uint8_t waveBuffer[GBWAVESIZE/2];
    int waveOffset = (waveNumber*16)+LSDJWAVESTARTADDRESS;
    memcpy(waveBuffer,&lsdjFile[waveOffset],GBWAVESIZE/2);
    wavetableTo4bit(waveBuffer,wave);

}

int getLoadedSongindex(char* filename){
    uint8_t songIndex;
    FILE* f = fopen(filename,"rb");
    fseek(f,0x8140,SEEK_SET);//active file
    fread(&songIndex,1,1,f);
    fclose(f);
    return songIndex;
}

void writeDMW(uint8_t* wavetable,char* filepath){
    char outfilepath[60];
    sprintf(outfilepath,"%s.dmw",filepath);
    uint8_t header[4] = {GBWAVESIZE};
    FILE* f;
    f = fopen(outfilepath,"wb");
    fwrite(header,4,sizeof(uint8_t),f);
    fputc(GBBITDEPTH,f);
    for(int i = 0; i < GBWAVESIZE; i++){
        fputc(wavetable[i],f);
    }
    fclose(f);
}

uint8_t** allocWavetableArray(int numWT){
    int size = 32 * numWT;
    uint8_t** waveTables;
    uint8_t* ptrBuffer;
    waveTables = (uint8_t**)malloc(size);
    //setup 2D
    ptrBuffer = (uint8_t*)(waveTables + numWT);
    for(int i = 0; i < numWT; i++){
        waveTables[i] = (ptrBuffer + 32 * i);
    }
    return waveTables;
}

void printAllFilenames(char* filepath){
    char strr[9];
    for(int i = 0; i < 20; i++){
        findNameFromFileindex(filepath,strr,i);
        printf("file%d: %s\n",i+1,strr);
    } 
}