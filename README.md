# SistemasOperacionais_threads
### Resumo

Este projeto visa a criação de um programa que utilize múltiplas threads para ler valores inteiros de N arquivos de entrada e armazenar em ordem crescente em um único arquivo de saída. Os dados de entrada não são necessariamente ordenados, dessa forma o programa deve ordena-los antes de coloca-los no arquivo de saída, além disso, o programa utiliza a biblioteca POSIX Threads, e deve ser utilizado para sistemas operacionais Linux.

### Solução do Problema 

Para solucionar o problema descrito acima, utilizou-se algumas das documentações disponibilizadas no moodle, principalmente os documentos utilizados na aula prática no laboratório. Dessa forma, iniciou-se o desenvolvimento do trabalho testando os materias disponíveis, para melhor compreensão de como funciona um programa com múltiplas threads.

###### Bibliotecas

No início do arquivo mergesort.c há a declaração das seguintes bibliotecas:

```c
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
```

As primeiras duas, são bibliotecas padrões normalmente utilizadas em quaisquer programas em C. A biblioteca <pthread> é a principal a ser utilizada neste trabalho, uma vez esta é um padrão POSIX para threads, a qual define uma API padrão para criar e manipular threads. E por último a <sys/time.h>.    

###### Funções e variáveis

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

#### O código

###### Funções

```c
void *thread_merge_sort(void* arg)
{
    int thread_id = (long)arg;
    int left = thread_id * (NUMBERS_PER_THREAD);
    int right = (thread_id + 1) * (NUMBERS_PER_THREAD) - 1;
    if (thread_id == NUM_THREADS - 1) {
        right += OFFSET;
    }
    int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort(arr, left, right);
        merge_sort(arr, left + 1, right);
        merge(arr, left, middle, right);
    }
    return NULL;
}
```

A função acima tem como objetivo realizar o próprio merge and sort, isto é ler os valores inteiros de vários arquivos e, de forma ordenada, armazená-los em um arquivo de saída. Para tanto, primeiro essa função obtém o id da thread, e procura o os limites tanto da esquerda como os da direita no vetor e em seguida já realiza o um merge and sort, ou seja, compara os valores armazenados  na variável right e na na variável left, como mostra a seguir:

```c
      int middle = left + (right - left) / 2;
    if (left < right) {
        merge_sort(arr, left, right);
        merge_sort(arr, left + 1, right);
        merge(arr, left, middle, right);
    }
```

Dessa forma, ele utiliza as funções merge e merge_sort para ordenar o vetor. Veremos o funcionamento das mesmas a seguir.

```c
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }		
}
```

Na função merge_sort é uma função recursiva, que utiliza como argumentos os limites que foram dados na função anterior, e um vetor.  

```c
void merge(int arr[], int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    int left_array[left_length];
    int right_array[right_length];
    
    /* copy values to left array */
    for (int i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }
    
    /* copy values to right array */
    for (int j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }
    
    i = 0;
    j = 0;
    /** chose from right and left arrays and copy */
    while (i < left_length && j < right_length) {
        if (left_array[i] <= right_array[j]) {
            arr[left + k] = left_array[i];
            i ++;
        } else {
            arr[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
    
    /* copy the remaining values to the array */
    while (i < left_length) {
        arr[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        arr[left + k] = right_array[j];
        k ++;
        j ++;
    }
}

```

A função acima é responsável  é responsável basicamente por juntar todos os vetores em um só, em seguida vemos como 

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







