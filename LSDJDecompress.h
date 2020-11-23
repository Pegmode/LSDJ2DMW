#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FILE_ALLOC_ADDRESSBASE 0x8141
#define FILE_ALLOC_SIZE 191
#define FILE_BLOCK_LENGTH 0x200
#define DEFAULT_DECOMPRESS_LENGTH 16
#define MAX_FILE_NUMBER 0x20
#define BLOCK_BASE_ADDRESS 0x8200
#define FILE_BLOCKS_SIZE 0x17DFF


/*
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
 * C: 8.7.7?
*/