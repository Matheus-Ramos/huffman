#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#define BYTE_SIZE 256
#define MAX_LENGHT 1000
typedef unsigned char _BYTE;
typedef unsigned long int _FREQUENCY_TYPE;
typedef struct node _node;
typedef struct queue _queue;
typedef struct bit_node _bit_node;
typedef struct dictionary _dictionary;

_queue* create_queue();

_node* create_node(_BYTE byte, _FREQUENCY_TYPE freq);

_dictionary* create_dictionary();

void write_compressed(_dictionary *dic, char *url, _node *huff_t);

void write_decompressed(FILE *file_r, char *url, _node *huff_t, int trash);

void priority_enqueue(_queue *queue, _node *node);

_node* dequeue(_queue *queue);

void print_queue(_queue *queue);

_node* create_huffman_tree(_queue *bfreq);

_queue* read_bytes_frequency(char *url);

void add_on_dictionary(_node *huff_t, _dictionary *new_dic, _bit_node **head);

void print_dictionary(_dictionary *d);

void print_tree(_node *bt);

void read_header(FILE *file_r, int *trash, int *tree_size, _node **huff_t);

_node* read_tree(FILE *file_r, int tree_size, _node **huff_t);


#endif /* HUFFMAN_TREE_H_ */
