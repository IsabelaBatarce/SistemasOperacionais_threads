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
int TAMANHO = 0;
#define UPPER_LIM 10000
#define LOWER_LIM  1
int NUM_THREADS;
int v[UPPER_LIM];
int NUMBERS_PER_THREAD;
int OFFSET;
void merge_sort(int v[], int inicio, int final);
void merge(int v[], int inicio, int meio, int final);
void* thread_merge_sort(void* arg);
void mesclando_subvetores(int v[], int numero, int aggregation);
void teste_vetor_ordenado(int v[]);
```

#### O código

###### Funções

```c
void *thread_merge_sort(void* arg)
{
    int thread_id = (long)arg;
    int inicio = thread_id * (NUMBERS_PER_THREAD);
    int final = (thread_id + 1) * (NUMBERS_PER_THREAD) - 1;
    if (thread_id == NUM_THREADS - 1) {
        final += OFFSET;
    }
    int meio = inicio + (final - inicio) / 2;
    if (inicio < final) {
        merge_sort(v, inicio, final);
        merge_sort(v, inicio + 1, final);
        merge(v, inicio, meio, final);
    }
    return NULL;
}
```


 A função acima tem como objetivo dividir os dados em subsequências pequenas,este passo é realizado recursivamente, iniciando com a divisão do vetor de n elementos em duas metades, cada uma das metades é novamente dividida em duas novas metades e assim por diante, até que não seja mais possível a divisão (ou seja, sobrem n vetores com um elemento cada).

Dessa forma, ele utiliza as funções merge e merge_sort para ordenar o vetor. Veremos o funcionamento das mesmas a seguir.

```c
void merge_sort(int v[], int inicio, int final) {
    if (inicio < final) {
        int meio = inicio + (final - inicio) / 2;
        merge_sort(v, inicio, meio);
        merge_sort(v, meio + 1, final);
        merge(v, inicio, meio, final);
    }
}
```

A função merge_sort é uma função recursiva, que utiliza como argumentos os limites que foram dados na função anterior. Sendo assim, o programa separa a função em duas e chama o merge_sort pra cada uma e por fim junta as duas com a função merge. 

A seguir na função merge, o programa pega os sub-vetores que ele recebe na chamada da função merge sort, e então vai separando-os em dois subvetores até que cada vetor seja dividido para apenas um elemento em cada, então ele compara esses valores e troca as posições deles, devolvendo para o vetor inicial . O código desta função inicia-se com a declaração das variáveis que definem os tamanhos dos vetores que serão utilizados. Posteriormente por meio do laço de repetição os valores do iniciais, isto é, a primeira metade do vetor (lembre-se ele foi subdividido em 2) são transferidos para o vetor_esquerdo, o mesmo acontece com o vetor direito, porém nesse caso seria a segunda metade.

```c
void merge(int v[], int inicio, int meio, int final) {
    int i = 0;
    int j = 0;
    int k = 0;
    int TAMANHO_inicio = meio - inicio + 1;
    int TAMANHO_direito = final - meio;
    int vetor_esquerdo[TAMANHO_inicio];
    int vetor_direito[TAMANHO_direito];

    /* copia os valores para o vetor esquerdo */
    for (int i = 0; i < TAMANHO_inicio; i ++) {
        vetor_esquerdo[i] = v[inicio + i];
    }

    /* copia os valores para  o vetor direito*/
    for (int j = 0; j < TAMANHO_direito; j ++) {
        vetor_direito[j] = v[meio + 1 + j]S;
    }

    i = 0;
    j = 0;
    /** Escolhendo valores entre o vetor esquerdo e o vetor direito*/
    while (i < TAMANHO_inicio && j < TAMANHO_direito) {
        if (vetor_esquerdo[i] <= vetor_direito[j]) {
            v[inicio + k] = vetor_esquerdo[i];
            i ++;
        } else {
            v[inicio + k] = vetor_direito[j];
            j ++;
        }
        k ++;
    }

    /* Copiando os valores que restam */
    while (i < TAMANHO_inicio) {
        v[inicio + k] = vetor_esquerdo[i];
        k ++;
        i ++;
    }
    while (j < TAMANHO_direito) {
        v[inicio + k] = vetor_direito[j];
        k ++;
        j ++;
    }

```

Na função abaixo, junta os vetores locais (que já estão ordenados), para formar um array global. 

```c
  void mesclando_subvetores(int v[], int numero, int aggregation) {
    for(int i = 0; i < numero; i = i + 2) {
        int inicio = i * (NUMBERS_PER_THREAD * aggregation);
        int final = ((i + 2) * NUMBERS_PER_THREAD * aggregation) - 1;
        int meio = inicio + (NUMBERS_PER_THREAD * aggregation) - 1;
        if (final >= TAMANHO) {
            final = TAMANHO - 1;
        }
        merge(v, inicio, meio, final);
    }
    if (numero / 2 >= 1) {
        mesclando_subvetores(v, numero / 2, aggregation * 2);
    }
}
```

Falar sobre a main 

```c
int main(int argc, const char * argv[]) {

    struct timeval  start, end;
    double time_spent;

    if(argc<2){
        printf("faltam argumentos /n");
    }
    else{
        NUM_THREADS = strtol (argv[1],NULL,10);

        pthread_t threads[NUM_THREADS];
        gettimeofday(&start, NULL);
        //pegar dos arquivos aqui
        for(int a=2; a<argc;a++){
			printf("argv %s\n",argv[a]);
			FILE *fp = fopen(argv[a],"r");
				while(!feof(fp)){
				fscanf(fp,"%d\n",&v[TAMANHO++]);
				}
			fclose(fp);
		}

		NUMBERS_PER_THREAD = TAMANHO / NUM_THREADS;
        OFFSET = TAMANHO % NUM_THREADS;

        for (long i = 0; i < NUM_THREADS; i ++) {
            int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }

        for(long i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        mesclando_subvetores(v, NUM_THREADS, 1);


        gettimeofday(&end, NULL);
        time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +
                                (double) (end.tv_sec - start.tv_sec)));
        printf("Tempo gasto na execucao: %f segundos\n", time_spent);
        printf("Vetor ordenado:");
        for (int i = 0; i < TAMANHO; i ++) {
            printf("%d ",v[i]);
        }
	FILE *fp;	
	fp = fopen("saida.txt","w");
	for (int i = 0; i < TAMANHO; i ++) {
            fprintf(fp,"%d ",v[i]);
        }
		fclose(fp);
    }



    /* teste para garantir que o vetor está ordenado*/
    /* teste_vetor_ordenado(v); */
    return 0;
}
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







