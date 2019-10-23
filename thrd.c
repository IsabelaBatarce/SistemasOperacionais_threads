/** 
 * Este e um programa simples para exemplificar a utilizacao 
 * de Threads.
 *
 * Desenvolvido por: 
 *   Prof. Andre Leon S. Gradvohl, Dr.
 * 
 * Outros arquivos necessarios para a execucao desse 
 * programa sao:
 * - funcoes.h
 * - funcoes.c
 *
 * Ultima atualizacao:
 *   04/04/2019
 */
#include <stdio.h>
#include <unistd.h>  // Cabecalho para a funcao sleep
#include <pthread.h> // Cabecalho especifico para threads POSIX
#include "funcoes.h" // Cabecalho para a funcoes que serao os threads.

pthread_t meutid;        /* Variavel que armazena o id do thread principal */
pthread_t outrosTIDs[2]; /* Vetor que armazena o id dos outros threads */

int main( void )
{
 meutid = pthread_self(); // Funcao que captura o id do thread.
 printf ("Meu Thread ID  = %ld\n",meutid);

 printf("Disparando Thread sub_a");
/**
 * A funcao a seguir cria um thread (linha de execucao) para a funcao "sub_a".
 * O prototipo da funcao e:
 *   int pthread_create(pthread_t *thread,  const  pthread_attr_t *attr, 
 *                      void *(*start_routine, void*), void *arg);
 * Onde:
 *   "thread" e o identificador do thread que se quer criar.
 *   "attr" sao os atributos do Thread (Geralmente NULL).
 *   "start_routine" e a funcao onde estao os threads.
 *   "arg" sao os parametros da "start_routine".
 * A funcao retorna
 */
 pthread_create(&outrosTIDs[0], NULL, sub_a, NULL); 
 printf("(id = %ld)\n", outrosTIDs[0]);

 printf("Disparando Thread sub_b");

/**
 * A funcao a seguir cria um thread (linha de execucao) para a funcao "sub_b".
 */
 pthread_create(&outrosTIDs[1], NULL, sub_b, NULL);
 printf("(id = %ld)\n", outrosTIDs[1]);

 printf("Aguardando finalizacao dos Threads id=%ld e id=%ld\n", 
        outrosTIDs[0], outrosTIDs[1]);

/**
 * A funcao a seguir bloqueia o processo ate que o thread indicado termine.
 * O prototipo da funcao e:
 *   int pthread_join(pthread_t thread, void **value_ptr);
 * Onde:
 *   "thread" e o identificador do thread que se espera terminar.
 *   "value_ptr" e o valor de retorno da funcao
 * A funcao retorna 0 se funcionou corretamente e um valor 
 * diferente de 0 para indicar erro.
 */
 pthread_join(outrosTIDs[1], NULL);
 pthread_join(outrosTIDs[0], NULL);
 printf("Threads id=%ld e id=%ld finalizados\n", outrosTIDs[0], outrosTIDs[1]);

 return 1;
}
