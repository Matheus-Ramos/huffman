#include <stdio.h>
#include <stdlib.h>

#define MAX 10000

typedef struct Element
{
	int priority;
}element;

typedef struct Priority_Queue
{
	int current_size;
	element data[MAX];
}pq;

pq* create()
{
	pq *queue = (pq*) malloc(sizeof(pq));
	queue->current_size = 0;

	return queue;
}

int enqueue(pq *queue,int prty)
{
	int comparisons = 1;

	if(queue->current_size >= MAX)
	{
		printf("The queue is full\n");
	}
	else
	{
		int i = queue->current_size-1;

		//O fim do array é o inicio da fila,então que tiver maior prioridade irá para o final
		while(i>=0 && queue->data[i].priority >= prty )
		{
			//esse while irá encontrar a posição "i" onde a prioridade que nós queremos inserir vai ficar na fila e também mudar as posições "se necessário" de quem já está na fila
			queue->data[i+1] = queue->data[i];
			i--;
			comparisons++;
		}
		queue->data[i+1].priority = prty;
		queue->current_size++;

	}

	return comparisons;
}

void dequeue(pq *queue)
{
	if(queue->current_size == 0)
	{
		printf("The queue is empty\n");
	}
	
	queue->current_size--;
}

void swap(int *p,int *q)
{
	int aux;

	aux = *p;
	*p = *q;
	*q = aux;

	return;
}
void quicksort(int *v,int size)
{
	if(size <= 1)
	{
		return;
	}
	else
	{
		int middle = size/2;
		int pivot = v[middle];
		int a = 0,b = size-1;

		while(a < b)
		{
			while(v[a] <= pivot && a < middle)a++;
			while(v[b] >= pivot && b > middle)b--;

			if(a < b && v[a] != v[b])
			{
				if(a == middle) middle = b;
				else if(b == middle) middle = a;
				swap(&v[a],&v[b]);
			}
		}
		quicksort(v,b);
		quicksort(v+a,size-a);
	}
}


int main()
{
    pq *queue = create();
    FILE *out = fopen("Priority_Queue.txt","w");//arquivo de saida com as comparações
    int i,count,comparisons[1000],numb;

    for(count = 0;count < 1000;count++)
    {
    	scanf("%d",&numb);
    	comparisons[count] = enqueue(queue,numb);
    }

    quicksort(comparisons,1000);//ordenar as comparações do array

    for(count = 0;count < 1000;count++)
    {
    	i = comparisons[count];
    	fprintf(out,"%d,",i);//vai printar cada comparação no arquivo
    }

    fclose(out);
    printf("fim\n");

	return 0;
}