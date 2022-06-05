#include "compression.h"
#include "miniz.h"
#include <stdio.h>


int8_t compress_data(uint8_t** output_data, uint32_t* output_size, uint8_t* input_data, uint32_t input_size, uint32_t compression_level)
{
	uint8_t* compressed_data = NULL;
	mz_ulong compressed_size = mz_compressBound(input_size);

	compressed_data = malloc(compressed_size);

	if (compress2(compressed_data, &compressed_size, input_data, input_size, compression_level) != MZ_OK)
		return -1;

	*output_data = compressed_data;
	*output_size = compressed_size;

	return 0;
}

int8_t compress_file( char* file_path, uint8_t** compressed_data , uint32_t* compressed_size, uint32_t level )
{
	FILE* file = NULL;
		
	fopen_s(&file,file_path, "rb");
	fseek(file, SEEK_CUR, SEEK_END);

	uint32_t file_size = ftell(file);

	printf("File : %s \n", file_path);
	printf("File Size=%d Bytes\n", file_size);

	uint8_t* file_buff = calloc(file_size,1);

	rewind(file);

	if (file_buff != NULL)
		fread(file_buff, 1, file_size, file);

	if (compress_data(compressed_data, compressed_size, file_buff, file_size, level ) == -1)
		return -1;

	printf("Compressed Size = %d Bytes\n\n", *compressed_size);

	free(file_buff);

	return 0;
}