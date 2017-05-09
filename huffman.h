#ifndef HUFFMAN_H_
#define HUFFMAN_H_

/*
 *	Recebe uma fila de prioridade que
 *	corresponde a frequência com que os
 *	bytes se repetem no arquivo de origem
 *	e retorna a árvore de huffman.
 */
int huffman_compress(char *url);

/*
 *	Recebe a url de um arquivo e conta
 *	com que frequencia seus bytes se
 *	repetem, criando uma fila de proridade.
 */
int huffman_decompress(char *url);

#endif /* HUFFMAN_H_ */
