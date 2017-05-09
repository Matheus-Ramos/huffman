#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Gera os números aleatórios de 0 a 1000.
int main()
{
	int array[1000],i,d;

	FILE *out = fopen("input.in","w");

	srand(time(NULL));

	for(i = 0;i<1000;i++)
    {
    	array[i] = rand() %1000;
    }

    for(i = 0;i<1000;i++)
    {
        d = array[i];
    	fprintf(out,"%d ",d);
    }

    fclose(out);
}