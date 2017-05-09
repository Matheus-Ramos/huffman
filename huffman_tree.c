//............................................................................................(includes_begin)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//............................................................................................(\includes_end)
//............................................................................................(declarações_begin)
#define BYTE_SIZE 256
#define MAX_LENGHT 1000
typedef unsigned char _BYTE;
typedef unsigned long int _FREQUENCY_TYPE;
typedef struct node _node;
typedef struct queue _queue;
typedef struct bit_node _bit_node;
typedef struct dictionary _dictionary;

struct node
{
	_FREQUENCY_TYPE freq;
	_BYTE byte;
	struct node *next;
	struct node *left;
	struct node *right;
};

struct bit_node
{
	_BYTE bit;
	struct bit_node *next;
};

struct dictionary
{
	struct bit_node *dicionary[BYTE_SIZE];
};

struct queue
{
	struct node *head;
};

int is_empty(_node *huff_t)
{
	return huff_t == NULL;
}
int is_leaf(_node *huff_t)
{
	if(!is_empty(huff_t))
		return ( huff_t->left == NULL && huff_t->right == NULL );
	return -1;
}
//............................................................................................(\declarações_end)
_bit_node* add_bit(_bit_node *head, _BYTE bit)
{
	_bit_node *new_bit = (_bit_node*) malloc(sizeof(_bit_node));
	new_bit->next = head;
	new_bit->bit = bit;
	return new_bit;
}

_bit_node* remove_first_bit(_bit_node *head)
{
	_bit_node *aux = head;
	head = head->next;
	free(aux);
	return head;
}

//............................................................................................(criar FILA_begin)
_queue* create_queue()
{
	_queue *new_queue = (_queue*) malloc(sizeof(_queue));
	new_queue->head = NULL;
	return new_queue;
}
//............................................................................................(\criar FILA_end)
//............................................................................................(criar NÓ_begin)
_node* create_node(_BYTE byte, _FREQUENCY_TYPE freq)
{
	_node *new_huff_t = (_node*) malloc(sizeof(_node));
	new_huff_t->byte = byte;
	new_huff_t->freq = freq;
	new_huff_t->next = NULL;
	new_huff_t->left = NULL;
	new_huff_t->right = NULL;

	return new_huff_t;
}
//............................................................................................(\criar NÓ_end)
//............................................................................................(enfileirar ordenado_begin)
void priority_enqueue(_queue *queue, _node *node)
{
	_node *previous = NULL;
	_node *current = queue->head;
	if(current == NULL)							//caso não exista nada na fila
	{
		queue->head = node;							//node será a cabeça da fila
	}
	else
		while(current != NULL)
		{
			if(current->freq >= node->freq)
			{
				node->next = current;			//o node->next apontará para o current
				if(previous == NULL)			//caso node seja inserido no inicio da fila
					queue->head = node;				//node sera a nova cabeça

				else							//caso node seja inserido no meio da fila
					previous->next = node;		//previous->next apontará para o node
												//a cabeça continuará sendo head
				return;
			}
			else if( current->next == NULL )	//caso node seja inserido no fim da fila
			{
				current->next = node;			//o ultimo item da fila irá apontar para node
				return;							//a cabeça continuará sendo head
			}
			previous = current;
			current = current->next;
		}
}
//............................................................................................(\enfileirar ordenado_end)
//............................................................................................(desenfileirar_begin)
_node* dequeue(_queue *queue)
{
	_node *node;

	if(queue->head == NULL)						//se a fila estiver vazia retorna NULL
		return NULL;
	else										//se não
	{
		node = queue->head;						//pego o nó que estava na cabeça da fila
		queue->head = queue->head->next;		//atualizo o head da fila
		node->next = NULL;						//tiro as referencias do next do nó pra não ter nenhum problema futuro
		return node;							//retorno o nó que peguei.
	}
}
//............................................................................................(\desenfileirar_end)
//............................................................................................(imprimir fila_begin)
void print_queue(_queue *queue)
{
	_node *head = queue->head;
	while(head != NULL)
	{
		printf("BYTE ['%c']\tFREQUENCIA[%8ld]\n",head->byte, head->freq);
		head = head->next;
	}
}

void print_list(_bit_node *node)
{
	while(node != NULL)
	{
		printf("%d",node->bit);
		node = node->next;
	}
}

void print_tree(_node *bt)
{
	if (bt != NULL)
	{
		printf("%c ( ", bt->byte);
		print_tree(bt->left);
		printf(", ");
		print_tree(bt->right);
		printf(" )  ");

	}
}

void print_dictionary(_dictionary *d)
{
	int i;
	for(i = 0; i < BYTE_SIZE; i++)
	{
		if(d->dicionary[i] != NULL)
		{
			printf("\n[%c]:", i);
			print_list(d->dicionary[i]);
		}
	}
}
//............................................................................................(\imprimir fila_end)
//............................................................................................(pegar frequencia dos bytes_begin)
/*
 *	Recebe a url de um arquivo e conta
 *	com que frequencia seus bytes se
 *	repetem, criando uma fila de proridade.
 */
_queue* read_bytes_frequency(char *url)
{
	_queue *p_queue = create_queue();					//fila de prioridade
	_FREQUENCY_TYPE freq[BYTE_SIZE] = {0};				//array para contar a frequencia dos bytes
	int byte, i;
	FILE *file = fopen(url, "rb"); 						//abre o arquivo

	while((byte = fgetc(file)) != EOF ){freq[byte]++;}	//conta no array a frequencia dos bytes
	fclose(file);										//fecha o arquivo.

	for( i = 0; i < BYTE_SIZE; i++)						//adiciona as frequencias dos bytes na estrutura
	{													//huffman_tree na forma de fila
		if(freq[i] > 0)									//mas só as frequencias acima de 0
		{ 											// \/ está criando um node do tipo _huffman_tree
													// \/ com byte i e freq[i].
			priority_enqueue(p_queue, create_node(i, freq[i]));
										//   FILA,     ITEM A SER ADICIONADO
		}
	}
	return p_queue;
}
//............................................................................................(\pegar frequencia dos bytes_end)
//............................................................................................(criar arvore de huffman_begin)
/*
 *	Recebe uma fila de prioridade que
 *	corresponde a frequência com que os
 *	bytes se repetem no arquivo de origem
 *	e retorna a raiz da árvore de huffman.
 */

_node* create_huffman_tree(_queue *bfreq)
{
	_node *left, *right, *new_node;
	while(bfreq->head->next != NULL)								//enquanto tiver mais de um elemento na lista.
	{
		left = dequeue(bfreq);								//desenfileiro um para colocar na esquerda do nó
		right = dequeue(bfreq);								//desenfileiro outro pra colocar na direita do nó
		new_node = create_node('*', (left->freq + right->freq));	//crio o nó com simbolo de '*' e frequencia a soma das frequencias de left e right
		new_node->left = left;								//coloco left na esquerda do nó
		new_node->right = right;							//coloco right na direita do nó
		priority_enqueue(bfreq, new_node);					//enfileiro o nó na Fila.
	}
	return bfreq->head;										//retorno a raiz da arvore
}
//............................................................................................(\criar arvore de huffman_end)

_dictionary* create_dictionary()
{
	_dictionary *new_dic = (_dictionary*) malloc(sizeof(_dictionary));
	int i;
	for (i = 0; i < BYTE_SIZE; i++)
	{
		new_dic->dicionary[i] = NULL;
	}
	return new_dic;
}


void add_on_dictionary(_node *huff_t, _dictionary *new_dic, _bit_node **head)
{
	_bit_node *aux = *head;
	_bit_node **folha;
	if(is_leaf(huff_t))
	{
		while(aux != NULL)
		{
			folha = &new_dic->dicionary[ huff_t->byte ];
			*folha = add_bit(*folha, aux->bit);
			aux = aux->next;
		}
	}else{
		*head = add_bit(*head, 0);
		add_on_dictionary(huff_t->left, new_dic, head);
		*head = remove_first_bit(*head);
		*head = add_bit(*head, 1);
		add_on_dictionary(huff_t->right, new_dic, head);
		*head = remove_first_bit(*head);
	}
}

void calc_bits(_node *huff_t, int *nbits, int *total)
{
	if(is_leaf(huff_t))
		{
			(*total) += ((*nbits) * (huff_t->freq))%8;
		}
		else
		{
			(*nbits)++;
			calc_bits(huff_t->left, nbits, total);
			calc_bits(huff_t->right,nbits, total);
			(*nbits)--;
		}
}

int calc_trash(_node *huff_t)
{
	int nbits = 0, total = 0;

	calc_bits(huff_t, &nbits, &total);
	return 8 - (total%8);
}

void calc_tree(_node *huff_t, int *count)
{
	if (huff_t != NULL)
	{
		(*count)++;
		calc_tree(huff_t->left, count);
		calc_tree(huff_t->right,count);

		if(is_leaf(huff_t))
		{
			if(huff_t->byte == '*' || huff_t->byte == '\\')
				(*count)++;
		}
	}
}

_BYTE set_bit(_BYTE c, int i)
{
	_BYTE mask = 1 << i;
	return mask | c;
}

int is_bit_i_set(_BYTE c, int i)
{
	_BYTE mask = 1 << i;
	return mask & c;
}

_BYTE byte_empty()
{
	return (_BYTE) 0;
}

void write_tree_on_header(_node *huff_t, FILE *file_w)
{
	if(huff_t != NULL)
	{
		if(is_leaf(huff_t))
		{
			if(huff_t->byte == 42 || huff_t->byte == 92)
				fputc( 92, file_w );
		}
		fputc( huff_t->byte, file_w );
		write_tree_on_header(huff_t->left, file_w);
		write_tree_on_header(huff_t->right,file_w);
	}
}

void read_tree(FILE *file_r, int tree_size, _node **huff_t)
{
	_BYTE item;
	int folha = 0;

	if(tree_size > 0)
	{
		//ler do arquivo
		item = fgetc(file_r);
		//se lermos uma barra pula
		if(item == '\\')
		{
			item = fgetc(file_r);
			folha = 1;
			tree_size--;
		}
		//crio o nó
		(*huff_t) = (_node*) malloc(sizeof(_node));
		(*huff_t)->byte = item;
		(*huff_t)->left = NULL;
		(*huff_t)->right = NULL;
		tree_size--;

		//se não for uma folha os filhos são criados
		if( !folha && item == '*' )
		{
			read_tree(file_r, tree_size, &((*huff_t)->left));
			read_tree(file_r, tree_size, &((*huff_t)->right));
		}
	}
}

void read_header(FILE *file_r, int *trash, int *tree_size, _node **huff_t)
{
	_BYTE byte;

/*
 * 	lendo primeiro byte do arquivo para pegar o trash_size
 */
	byte = fgetc(file_r);
	(*trash) = byte >> 5;

/*
 * 	ainda no primeiro byte vamos pegar os 5 bits da direita que correspondem a uma parte de tree_size
 * 	lemos segundo byte do arquivo para pegar o tree_size
 * 	31 = mascara de bits 0001 1111
 */
	(*tree_size) = ((31) & byte) << 8;
	byte = fgetc(file_r);
	(*tree_size) |= byte;

	read_tree(file_r, (*tree_size), huff_t);
}

void write_header(FILE *file_w, int tree_s, int trash, _node *huff_t)
{
	_BYTE byte = byte_empty();
/*
 * 	Colocando trash nos primeiros 3 bits (3 bits da esquerda).
 *	Junto dele os bits excedentes de tree_size.
 *	224 equivale a mascara 1110 0000
 *	31  equivale a mascara 0001 1111
 *	só pra garantir que tree_size não escreva por cima do trash_size
 *	e vice versa
 */
	byte = ( (224) & (trash << 5)) | ( (31) & (tree_s >> 8));
	fputc(byte, file_w);
/*
 * Colocando os 8 bits de tree_size que sobraram de tree_size.
 * 255 = mascara 1111 1111
 * para escrever só os 8 bits mais a direita de tree_size.
 */
	byte = byte_empty();
	byte = (tree_s & (255));
	fputc(byte, file_w);

/*
 * escrevendo arvore no arquivo
 */
	write_tree_on_header(huff_t, file_w);
}

void write_compressed(_dictionary *dic, char *url, _node *huff_t)
{
	char url_w[MAX_LENGHT];
	int tree_size = 0;
	int pos, byte_f;
	_bit_node *current;
	_BYTE byte;

	strcpy(url_w, url);
	strcat(url_w, ".huff");
	int trash = calc_trash(huff_t);
	calc_tree(huff_t, &tree_size);
	FILE *file_r = fopen(url, "rb");								//abrir arquivos de leitura
	FILE *file_w = fopen(url_w, "wb");					//e arquivo de escrita.

	write_header(file_w, tree_size, trash, huff_t);

	byte_f = fgetc(file_r);
	current = dic->dicionary[ byte_f ];

	while(byte_f != EOF)
	{
		byte = byte_empty();
		for(pos = 7; pos >= 0 && byte_f != EOF;)
		{

			if(current != NULL)
			{
				if(current->bit) 		//current->bit contém 1 ou 0
				{
					byte = set_bit(byte,pos);
				}
				pos--;
				current = current->next;
			}
			else
			{
				byte_f = fgetc(file_r);
				if(byte_f != EOF)
					current = dic->dicionary[ byte_f ];
			}
		}
		fputc(byte, file_w);
	}
	fclose(file_r);
	fclose(file_w);
}

void write_decompressed(FILE *file_r, char *url, _node *huff_t, int trash)
{
	char url_w[MAX_LENGHT];
	strcpy(url_w, url);
	url_w[ strlen(url_w) - 5] = '\0';

	int byte_f, next_byte_f, end, pos;
	_node *huff_t_aux = huff_t;

	FILE *file_w = fopen(url_w, "wb");					//e arquivo de escrita.

	/*
	 * ler byte do arquivo, navega pelos bits:
	 * quando chegar ao ultimo bit do byte, ler outro byte do arquivo.
	 * a cada bit 1 entra right na arvore.
	 * a cada bit 0 entra left na arvore.
	 * ao encontrar uma folha da arvore, escreva-a no arquivo e volte para a raiz da arvore.
	 */
	for(byte_f = fgetc(file_r), next_byte_f = fgetc(file_r); byte_f != EOF;)
	{
		for(pos = 7, end = 0; pos >= end && byte_f != EOF; pos--)
		{
			if(next_byte_f == EOF)
				end = trash;

			if( is_bit_i_set(byte_f, pos) ) 	//se o bit é 1
			{
				huff_t_aux = huff_t_aux->right;
			}
			else{								//se o bit é 0
				huff_t_aux = huff_t_aux->left;
			}

			if(is_leaf(huff_t_aux))
			{
				fputc(huff_t_aux->byte, file_w);
				huff_t_aux = huff_t;
			}
		}
		byte_f = next_byte_f;
		if(next_byte_f != EOF)
			next_byte_f = fgetc(file_r);

	}
	fclose(file_r);
	fclose(file_w);

}

