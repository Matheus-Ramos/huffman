#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"

typedef struct node
{
	int item;
	struct node *next;
}Node;

typedef struct stack
{
	Node *top;
}Stack;


int is_empty(Stack *stack)
{
	return (stack->top == NULL);
}

Stack* create_stack()
{
	Stack *new_stack = (Stack*) malloc(sizeof(Stack));
	new_stack->top = NULL;
	return new_stack;
}

void push(Stack *stack, int item)
{
	Node *new_top = (Node*) malloc(sizeof(Node));
	new_top->item = item;
	new_top->next = stack->top;
	stack->top = new_top;
}

int pop(Stack *stack)
{
	if (is_empty(stack))
	{
		return -1;
	}
	else
	{
		Node* guard = stack->top;
		stack->top = stack->top->next;
		return guard->item;
	}
}

int peek(Stack *stack)
{
	if (is_empty(stack))
	{
		return -1;
	}
	else
	{
		return stack->top->item;
	}
}

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}

void testSTACK(void)
{
	int i;
	Stack* pilha = create_stack();

	//função peek e pop retorna -1 caso a pilha esteja vazia (top == NULL)
	CU_ASSERT(-1 == peek(pilha));
	CU_ASSERT(-1 == pop(pilha));

	//adicionando números a pilha
	for (i = 1; i <= 1000; ++i)
	{
		push(pilha,i);
	}

	for (i = 1000; i >= 1; --i)
	{
		CU_ASSERT(i == peek(pilha));
		CU_ASSERT(i == pop(pilha));
	}

	//pilha vazia
	CU_ASSERT(-1 == peek(pilha));
	CU_ASSERT(-1 == pop(pilha));
}


int main()
{

   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of STACK()", testSTACK)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}