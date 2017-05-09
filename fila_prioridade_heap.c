#include <stdio.h>
#include <stdlib.h>

#define MAX 10000

typedef struct Element
{
	int priority;
}element;

typedef struct Priority_queue_heap
{
  int current_size;
  element data[MAX];
}pqh;

pqh* create()
{
	pqh *queue = (pqh*) malloc(sizeof(pqh));
	queue->current_size = 0;

	return queue;
}

int get_parent_index(pqh *queue,int i)
{
	return i/2;
}

int get_left_index(pqh *queue,int i)
{
	return 2*i;
}
int get_right_index(pqh *queue,int i)
{
	return 2*i + 1;
}


int enqueue(pqh *queue,int prty)
{
	int comparisons = 1;

	if(queue->current_size >= MAX)
	{
		printf("The queue is Full\n");
	}
	else
	{
		queue->current_size++;
		queue->data[queue->current_size].priority = prty;

		int key_index = queue->current_size;
		int parent_index = get_parent_index(queue,queue->current_size);

		while(parent_index >= 1 && queue->data[key_index].priority > queue->data[parent_index].priority)
		{
			// como a inserção na heap é feita na folha,se a prioriodade inserida for maior que a dos seus pais,a função vai trocar de posição essas prioridades
			element aux;
			aux = queue->data[parent_index];
			queue->data[parent_index] = queue->data[key_index];
			queue->data[key_index] = aux;

			key_index = parent_index;
			parent_index = get_parent_index(queue,key_index);

			comparisons++;
		}
	}

	return comparisons;
}

void max_heapify(pqh *queue,int i)
{
	int largest;
	int left_index = get_left_index(queue,i);
	int right_index = get_right_index(queue,i);

	if(left_index <= queue->current_size && queue->data[left_index].priority > queue->data[i].priority)
	{
		largest = left_index;
	}
	else
	{
		largest = i;
	}
	if(right_index <= queue->current_size && queue->data[right_index].priority > queue->data[largest].priority)
    {
    	largest = right_index;
    }
    if(queue->data[i].priority != queue->data[largest].priority)
    {
    	element aux;
    	aux = queue->data[i];
    	queue->data[i] = queue->data[largest];
    	queue->data[largest] = aux;

    	max_heapify(queue,largest);
    }
}

void dequeue(pqh *queue)
{
	if(queue->current_size == 0)
	{
		printf("The queue is empty\n");
	}
	else
	{
		queue->data[1] = queue->data[queue->current_size];
		queue->current_size--;
		max_heapify(queue,1);
	}
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
	pqh *queue = create();
    FILE *out = fopen("Heap.txt","w");
	int i,count,comparisons[1000],numb;

	for(count = 0;count < 1000;count++)
	{
		scanf("%d",&numb);
		comparisons[count] = enqueue(queue,numb);
	}

	quicksort(comparisons,1000);

	for(count = 0;count < 1000;count++)
	{
		i = comparisons[count];
		fprintf(out,"%d,",i);
	}

	fclose(out);
	return 0;
}