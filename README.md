# SistemasOperacionais_threads
### Resumo

Este projeto visa a criação de um programa que utilize múltiplas threads para ler valores inteiros de N arquivos de entrada e armazenar em ordem crescente em um único arquivo de saída. Os dados de entrada não são necessariamente ordenados, dessa forma o programa deve ordena-los antes de coloca-los no arquivo de saída, além disso, o programa utiliza a biblioteca POSIX Threads, e deve ser utilizado para sistemas operacionais Linux.

### Solução do Problema 

Para solucionar o problema descrito acima, utilizou-se algumas das documentações disponibilizadas no moodle, principalmente os documentos utilizados na aula prática no laboratório. Dessa forma, iniciou-se o desenvolvimento do trabalho testando os materias disponíveis, para melhor compreensão de como funciona um programa com múltiplas threads.

No início do arquivo mergesort.c há a declaração das seguintes bibliotecas:

```c
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
```

As primeiras duas, são bibliotecas padrões normalmente utilizadas em quaisquer programas em C. A biblioteca <pthread> é a principal a ser utilizada neste trabalho, uma vez esta é um padrão POSIX para threads, a qual define uma API padrão para criar e manipular threads. E por último a <sys/time.h>    

Posteriormente, definimos algumas variáveis e funções para o programa:

```c
int LENGTH = 0;
#define UPPER_LIM 10000
#define LOWER_LIM  1
int NUM_THREADS;
int arr[UPPER_LIM];
int NUMBERS_PER_THREAD;
int OFFSET;
int generate_random_number(unsigned int lower_limit, unsigned int upper_limit);
void merge_sort(int arr[], int left, int right);
void merge(int arr[], int left, int middle, int right);
void* thread_merge_sort(void* arg);
void merge_sections_of_array(int arr[], int number, int aggregation);
void test_array_is_in_order(int arr[]);
```



### Compilação 

Para a compilação do programa, é necessário utilizar o prompt de comando de um computador com o sistema operacional Linux. Após abrir a tela do terminal, você deverá utilizar o comando cd <nome do arquivo> até entrar no diretório do código.  Veja na imagem abaixo um exemplo: 

*******Imagem do terminal ***

Em seguida, digite o comando gcc *c, como na tela abaixo:

***Tela ***

Posteriormente digite o comando  ./a.out <numero de threads> <arquivo de entrada> pra executar. Não se esqueça que o arquivo de entrada deve conter apenas os números que serão ordenados no programa.

***Exemplo da execução ***

****Exemplo do arquivo de entrada****

Dessa forma, o programa está pronto para rodar, e irá criar um arquivo de saída como mostra a figura abaixo.

**Figura do arquivo de saída**

### Gráficos







