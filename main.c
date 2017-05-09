#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main( int size, char *argsv[ ] )
{
	if(!strcasecmp( argsv[1], "-c" ))
	{
		huffman_compress(argsv[2]);

	}else if(!strcasecmp( argsv[1], "-d" ))
	{
		huffman_decompress(argsv[2]);
	}else{
		printf("OPCAO INVALIDA!\n");
	}
	return 0;
}

