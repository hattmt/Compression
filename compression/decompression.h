#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include "types.h"

#define DECOMPRESSION_BUFFER_OUTPUT_SIZE 16


typedef int8_t (*decompression_output_cb_t)(uint8_t* data, uint32_t size);

//initialiser la d�compression
int8_t decompression_init(decompression_output_cb_t cb);


//buffer d'entr�e � d�compresser
int8_t decompression_input_buffer(uint8_t* data, uint32_t size);

void decompression_deinit(void);




#endif

