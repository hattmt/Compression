#include "decompression.h"
#include "miniz.h"
#include <assert.h>

typedef struct
{
	mz_stream stream;
	uint8_t out_buff[DECOMPRESSION_BUFFER_OUTPUT_SIZE];//buffer de sortie
	uint8_t out_buff_acc[DECOMPRESSION_BUFFER_OUTPUT_SIZE];//buffer d'accumulation ( avoir des blocs de 16 octets en sortie )
	decompression_output_cb_t cb;
	uint32_t acc_cntr;
}decompression_ctx_t;



static decompression_ctx_t ctx = { 0 };


int8_t decompression_init(decompression_output_cb_t cb)
{
	uint8_t status = 0;

	memset(&ctx.stream, 0, sizeof(ctx.stream));
	status = mz_inflateInit(&ctx.stream);
	
	if (status != MZ_OK) 
		return -1;

	ctx.cb = cb;

	return status;
}


int8_t decompression_input_buffer(uint8_t* data, uint32_t size)
{
	uint32_t status = 0;
	uint32_t output_size = 0;

	ctx.stream.avail_in = size;
	ctx.stream.next_in = data;

	do {
		ctx.stream.avail_out = DECOMPRESSION_BUFFER_OUTPUT_SIZE;
		ctx.stream.next_out = ctx.out_buff;
		status = inflate(&ctx.stream, Z_NO_FLUSH);

		switch (status) 
		{
		case Z_NEED_DICT:
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
 			(void)inflateEnd(&ctx.stream);
			return -1;
		}

		output_size = DECOMPRESSION_BUFFER_OUTPUT_SIZE - ctx.stream.avail_out;


		//former des paquets de la taille du buffer de decompression
		for (uint32_t i = 0; i < output_size; i++)
		{
			if (ctx.acc_cntr == DECOMPRESSION_BUFFER_OUTPUT_SIZE)
			{
				if (ctx.cb != NULL)
					if ( (ctx.cb)(ctx.out_buff_acc, DECOMPRESSION_BUFFER_OUTPUT_SIZE) == -1 )
						return -1;

				ctx.acc_cntr = 0;
			}

			ctx.out_buff_acc[ctx.acc_cntr] = ctx.out_buff[i];
			ctx.acc_cntr++;
		}

		//decompression terminée
		if (status == Z_STREAM_END)
		{
			(ctx.cb)(ctx.out_buff_acc, output_size);
			return 0;
		}

	} while (ctx.stream.avail_out == 0 || ctx.stream.avail_in != 0);

}

void decompression_deinit(void)
{
	inflateEnd(&ctx.stream);
}
