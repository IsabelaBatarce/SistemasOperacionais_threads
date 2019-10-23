/**
 * Codigo com as implementacoes das funcoes sub_a e sub_b
 *
 * Desenvolvido por:
 *   Prof. Andre Leon S. Gradvohl, Dr.
 *
 * Ultima atualizacao:
 *   04/04/2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "funcoes.h" // Cabecalho que contem o prototipo dessas funcoes

void *sub_a(void *arg)
{
  printf("Teste isaaaaaaa\n");
  pthread_exit(NULL);
}

void *sub_b(void *arg)
{
  register int j=1;
  register int tempoEspera;

  for (j=1;j<30;j+=2)
  {
    tempoEspera =(rand() % 3)+1; //tempo aleatorio 1, 2 ou 3 seg.
    printf("\njota = %d. Tempo de Espera: %d\n",j, tempoEspera);
    sleep(tempoEspera); //Dorme um tempo.
  }
  pthread_exit(NULL);
}
