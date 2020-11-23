#include "LSDJDecompress.h"

//info https://littlesounddj.fandom.com/wiki/File_Management_Structure
//https://littlesounddj.fandom.com/wiki/.sav_structure
//WARNING: If comparing outputted binaries to .sav, timers may be different
 uint8_t defaultInstrument[] = {0xa8,0x0,0x0,0xff,0x0,0x0,0x3,0x0,0x0,0xd0,0x0,0x0,0x0,0xf3,0x0,0x0};//length 16
 uint8_t defaultWave[] = {0x8e,0xcd,0xcc,0xbb,0xaa,0xa9,0x99,0x88,0x87,0x76,0x66,0x55,0x54,0x43,0x32,0x31};//length 16


int calculateBlockBase(int blockNumber){//base address in .sav memory map
    return (0x8000 + 0x200 * blockNumber);
}

int findFirstBlockFromFileIndex(int fileIndex ,char* filePath){
    FILE* f = fopen(filePath,"rb");
    uint8_t fileAllocationTable[FILE_ALLOC_SIZE] = {0xff};
    fseek(f,FILE_ALLOC_ADDRESSBASE,SEEK_SET);
    fread(fileAllocationTable,FILE_ALLOC_SIZE,1,f);   
    fclose(f);

    for(int i = 0; i < FILE_ALLOC_SIZE; i++){//search filealloc table for file index
        //block indexed from 1
        //printf("%i\n",fileAllocationTable[i]);
        if(fileAllocationTable[i] == fileIndex){
            return i+1;
        }
    }
    return -1;
}

int calculateRealAddress(int cPos){
    return 0x8200 + cPos;
}

void createFileBlocksBuffer(char* filePath, uint8_t* blockBuffer){
    FILE* f = fopen(filePath,"rb");
    fseek(f,BLOCK_BASE_ADDRESS,SEEK_SET);
    fread(blockBuffer,FILE_BLOCKS_SIZE,1,f);
    fclose(f);

    FILE* fz = fopen("debugout2.bin","wb");
    fwrite(blockBuffer,1,FILE_BLOCKS_SIZE,fz);
    fclose(fz);
}
int uncompressDefault(uint8_t* uncompressedFile,uint8_t* defaultContent,int uPos, int cPos, uint8_t* compressedFile){
    int loopLength = compressedFile[cPos];
    for(int i = 0; i < loopLength; i++){
        memcpy(&uncompressedFile[uPos],defaultContent,DEFAULT_DECOMPRESS_LENGTH);
        uPos += 16;
    }
    return uPos;
}

int calculateBlockOffsetFromBlockIndex(int blockIndex){//base offset
    return (blockIndex-1) * 0x200;
}


void decompressBlocks(int startBlockIndex,char* filePath,uint8_t* uncompressedFile){
    uint8_t compressedFile[FILE_BLOCKS_SIZE] = {0};
    createFileBlocksBuffer(filePath,compressedFile);
    int uPos = 0;//uncompressed position
    int cPos =  calculateBlockOffsetFromBlockIndex(startBlockIndex);//compressed positon
    for(;uPos < 0x8000;){
        //printf("%d\n",uPos );
        switch(compressedFile[cPos]){
            case 0xC0://RLE?s
                cPos++;
                if(compressedFile[cPos] != 0xC0){//IS RLE
                    uint8_t byteValue = compressedFile[cPos++];
                    uint8_t rleLength = compressedFile[cPos++];
                    for(int j = 0; j < (int)rleLength; j++){
                        //compressedFile[uPos++] = byteValue;     
                        uncompressedFile[uPos++] = byteValue;
                    }
                    //printf("RLE    val: %X   len: %d   c adr: %X    u adr: %X\n",byteValue,rleLength,calculateRealAddress(cPos - 3),uPos);
                }   
                else{//IS NOT RLE
                    uncompressedFile[uPos++] = 0xC0;
                    cPos++;
                }             
                break;
            case 0xE0://command
                cPos++;
                switch (compressedFile[cPos]){
                case 0xE0://not a command
                    uncompressedFile[uPos++] = 0xE0;
                    cPos++;
                    break;
                case 0xF1://default instrument
                    //printf("\nPasting Default instrument    c adr:%X    u adr:%X\n",calculateRealAddress(cPos),uPos);
                    cPos++;
                    uPos = uncompressDefault(uncompressedFile,defaultInstrument,uPos,cPos,compressedFile);
                    cPos++;
                    break;
                case 0xF0://default wavetable
                    //printf("\nPasting Default WT    c adr:%X    u adr:%X\n",calculateRealAddress(cPos),uPos);
                    cPos++;
                    uPos = uncompressDefault(uncompressedFile,defaultWave,uPos,cPos,compressedFile);
                    cPos++;
                    break;
                case 0xFF://end of file
                    //printf("\nFinished Decompression    c adr:%X    u adr:%X\n",calculateRealAddress(cPos),uPos);
                    return;
                    break;
                default://block Switch
                    //printf("\nSwitching Blocks    c adr:%X    u adr:%X\n",calculateRealAddress(cPos),uPos);
                    cPos = calculateBlockOffsetFromBlockIndex(compressedFile[cPos]);
                    //printf("    new c adr:%X\n",calculateRealAddress(cPos));
                    //printf("next: %X\n",compressedFile[cPos]);
                    break;
                }

                break;
            default://write byte
                uncompressedFile[uPos++] = compressedFile[cPos++];
                break;
        }
    }
}

void decompressLSDJFile(int fileIndex,char* fileName, uint8_t* uncompressedFile){
    int blockIndex = findFirstBlockFromFileIndex(fileIndex,fileName);
    //printf("BLOCK INDEX: %i\n",blockIndex);
    if(blockIndex == -1){
        //BLOCK INDEX NOT FOUND
        printf("Block Index not found\n");
    }
    else decompressBlocks(blockIndex,fileName,uncompressedFile);
}

