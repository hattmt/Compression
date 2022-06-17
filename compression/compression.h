#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <stdlib.h>
#include "types.h"


int8_t compress_data(uint8_t** output_data, uint32_t* output_size, uint8_t* input_data, uint32_t input_size, uint32_t compression_level);

int8_t compress_file( char* file_path, uint8_t** compressed_data, uint32_t* compressed_size, uint32_t level );

#endif
