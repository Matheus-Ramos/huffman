//............................................................................................(includes_begin)
#include <stdio.h>
#include <stdlib.h>
#include "huffman_tree.h"
//............................................................................................(\includes_end)
//............................................................................................(declara��es_begin)
int is_file(char *url)
{
	FILE *file = fopen(url, "r");
	if(file != NULL)
	{
		fclose(file);
		return 1;
	}
	fclose(file);
	return 0;
}
//............................................................................................(\declara��es_end)
//............................................................................................(compress�o de huffman_begin)
/*
 *	RECEBE a url de um arquivo e aplica
 *	a compress�o de huffman, gerando um
 *	arquivo comprimido no mesmo local,
 *	RETORNA 0 se n�o tiver exito e 1 se
 *	tiver.
 */
int huffman_compress(char *url)
{
	if(is_file(url))
	{
		_queue *bfreq = read_bytes_frequency(url);
		_bit_node *head_list = NULL;
		_node *huffman_tree = create_huffman_tree(bfreq);
		_dictionary *dic = create_dictionary();
		add_on_dictionary(huffman_tree, dic, &head_list);
		write_compressed(dic, url, huffman_tree);
		printf("PRONTO!");
	}else{
		printf("ARQUIVO INVALIDO!");
	}
	return 1;
}
//............................................................................................(\compress�o de huffman_end)
//............................................................................................(descompress�o de huffman_begin)
/*
 *	RECEBE a url de um arquivo comprimido com
 *	a compress�o de huffman e gera um arquivo
 *	descomprimido no mesmo local, RETORNA 0 se
 *	n�o tiver exito e 1 se tiver.
 */
int huffman_decompress(char *url)
{
	if(is_file(url))
	{
		FILE *file_r = fopen(url, "rb");
		int trash, tree_size;
		_node *huff_t = NULL;

		read_header(file_r, &trash, &tree_size, &huff_t);
		write_decompressed( file_r, url, huff_t, trash);
		printf("DONE!");
	}else{
		printf("ARQUIVO INVALIDO!");
	}

	return 1;
}
//............................................................................................(\descompress�o de huffman_end)
