#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "lista.h"
#include "pilha.h"
 
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
 
void testePilha(void) //função que executa os testes para pilha
{
    int i;
    Stack* pilha = create_stack();
 
    //função peek e pop retorna -1 caso a pilha esteja vazia (top == NULL)
    CU_ASSERT(-1 == peek(pilha));
    CU_ASSERT(-1 == pop(pilha));
    //verifica se a pilha está vazia, ou seja, se a funçao peek e pop retorna -1
 
    for (i = 1; i <= 1000; ++i)
    {
        push(pilha,i); //adiciona 1000 numéros na pilha
    }
 
    for (i = 1000; i >= 1; --i)
    {
        CU_ASSERT(i == peek(pilha)); //verifica o retorno da função peek
        CU_ASSERT(i == pop(pilha));  //verifica o retorno da função po
    }
 
    CU_ASSERT(-1 == peek(pilha));
    CU_ASSERT(-1 == pop(pilha));
    //verifica se a pilha está vazia
}
 
void testeLista(void) //função que executa os testes para lista
{
    int i;
    List* lista = create_linked_list();
 
    CU_ASSERT(0 == lista->size); //verifica se o tamanho da lista é 0
    CU_ASSERT(1 == is_empty_list(lista)); //verifica se a lista está vazia
 
    for (i = 1; i <= 1000; ++i)
    {
        add_node(lista,i); //adiciona 1000 números na lista
    }
 
    CU_ASSERT(1000 == lista->size); //verifica se o tamanho da ĺista é 1000
 
    for (i = 1000; i >= 1 ; --i)
    {
        CU_ASSERT(i == remove_head(lista));
        //verifica se a funçao remove_head retorna os números de 1000 a 1
    }
 
    CU_ASSERT(1 == is_empty_list(lista));//verifica se a lista está vazia
    CU_ASSERT(0 == lista->size);//verifica se o tamanho da lista é 0
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
   if ((NULL == CU_add_test(pSuite, "teste de Pilha", testePilha)) ||
       (NULL == CU_add_test(pSuite, "teste de Lista", testeLista)))
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