#include <stdio.h>
#include <stdlib.h>

#include "decompression.h"
#include "compression.h"

FILE* write = NULL;

//sortie par blocs de DECOMPRESSION_BUFFER_OUTPUT_SIZE octets (a part le dernier bloc)
int8_t decompression_output_buffer( uint8_t* data, uint32_t length)
{
	fwrite(data, 1,length, write);
	return 0;
}


int main(void)
{
	//compression
	uint8_t* compressed_data = NULL;
	uint32_t compressed_size = 0;

	fopen_s(&write,"stm32g4xx_hal_tim2.lst", "wb");

	compress_file("stm32g4xx_hal_tim.lst", &compressed_data, &compressed_size, 9 );

	// decompression
	decompression_init(decompression_output_buffer);

	unsigned char* cursor = NULL;

	//decompression par blocs de 16 octets

	for (uint32_t i = 0; i < compressed_size; i += 16)
	{
		cursor = &compressed_data[i];
		
		if( cursor != NULL)
			decompression_input_buffer( cursor, 16);
	}

	free(compressed_data);
}





