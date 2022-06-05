#include <stdio.h>
#include <stdlib.h>

#include "decompression.h"
#include "compression.h"


//sortie par blocs de DECOMPRESSION_BUFFER_OUTPUT_SIZE octets (a part le dernier bloc)
int8_t decompression_output_buffer( uint8_t* data, uint32_t length)
{
	char data2[DECOMPRESSION_BUFFER_OUTPUT_SIZE + 1] = { 0 };
	memcpy( data2, data, length);
	printf("%s || ", data2);
	return 0;
}


int main()
{
	//compression
	uint8_t* compressed_data = NULL;
	uint32_t compressed_size = 0;

	compress_file("test_compression.txt", &compressed_data, &compressed_size, 9 );

	// decompression
	decompression_init(decompression_output_buffer);

	unsigned char* cursor = NULL;

	//decompression par blocs de 16 octets
	for (int i = 0; i < compressed_size; i += 16)
	{
		cursor = &compressed_data[i];
		
		if( cursor != NULL)
			decompression_input_buffer( cursor, 16);
	}

	decompression_deinit();

	free(compressed_data);
}





