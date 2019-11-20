# SistemasOperacionais_threads
### Resumo

Este projeto visa a criação de um programa que utilize múltiplas threads para ler valores inteiros de N arquivos de entrada e armazenar em ordem crescente em um único arquivo de saída. Os dados de entrada não são necessariamente ordenados, dessa forma o programa deve ordena-los antes de coloca-los no arquivo de saída, além disso, o programa utiliza a biblioteca POSIX Threads, e deve ser utilizado para sistemas operacionais Linux.

### Solução do Problema 

 O método escolhido para solucionar o problema foi a técnica de merge and sort, muitas vezes traduzida para “dividir e conquistar” que consiste em lidar com um vetor inicial de inteiros e dividi-lo em várias partes menores, ordenando uma por uma e então ordenando estes subvetores de volta em um vetor principal. 

#### O código

###### Bibliotecas

No início do arquivo mergesort.c há a declaração das seguintes bibliotecas:

```c
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
```

As primeiras duas, são bibliotecas padrões normalmente utilizadas em quaisquer programas em C. A biblioteca <pthread> é a principal a ser utilizada neste trabalho, uma vez esta é um padrão POSIX para threads, a qual define uma API padrão para criar e manipular threads. E por último a biblioteca <sys/time.h>, a qual é utilizada para pegar o tempo do sistema já formatado.    

###### Funções e variáveis

Posteriormente, definimos algumas variáveis e funções para o programa:

```c
/* definindo as variaveis  */
int TAMANHO = 0;
#define UPPER_LIM 10000 /*Quantidade máxima de números que podem entrar no sistema*/
#define LOWER_LIM  1 /*Quantidade mínima para o programa rodar*/
int NUM_THREADS; /*Variável global para a quantidade de threads do sistema*/
int v[UPPER_LIM]; /*Vetor global utilizado para guardar todos os números dos arquivos de entrada*/
int NUMBERS_PER_THREAD; /*Quantidade de números designados para cada thread*/
int OFFSET; /*Utilizada na main para guardar o resto da divisão da quantidade de números para cada thread*/
/* Declarando as funções */
void merge_sort(int v[], int inicio, int final);
void merge(int v[], int inicio, int meio, int final);
void* thread_merge_sort(void* arg);
void mesclando_subvetores(int v[], int numero, int aggregation);
void teste_vetor_ordenado(int v[]);
```

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

 A função acima, é basicamente um roteiro do que cada thread deve seguir, primeiramente ela recebe o id da thread para encontrar os limites iniciais e finais do vetor, isto é, o primeiro e último elemento do vetor. Posteriormente, os dados são divididos em subsequências pequenas,este passo é realizado na função merge_sort, recursivamente, iniciando com a divisão do vetor de n elementos em duas metades, cada uma das metades é novamente dividida em duas novas metades e assim por diante, até que não seja mais possível a divisão, ou seja, sobrem n vetores com um elemento cada. Sendo assim, o programa subdivide os vetores em dois por meio da função merge_sort e por fim junta-os com a função merge. Veremos o funcionamento das funções merge e merge_sort a seguir. 

A função merge_sort é uma função recursiva, que utiliza como argumentos os limites que foram dados na função anterior. O programa pega os vetores que ele recebe na chamada da função, e então vai separando-os em dois sub-vetores até que cada vetor seja dividido para apenas um elemento em cada, no fim ele possuí dois vetores, um com o início e outro com o final, dessa forma, utiliza-se a função merge para juntar ambos em um vetor final. 

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

 O código abaixo, da função merge, inicia-se com a declaração das variáveis que definem os tamanhos dos vetores que serão utilizados. Posteriormente por meio do laço de repetição os valores iniciais, ou seja , a primeira metade do vetor são transferidos para o vetor_esquerdo, o mesmo acontece com o vetor direito, porém nesse caso seria a segunda metade. Por fim, os valores do vetor esquerdo e do vetor direito são comparados para ordena-los em um vetor final com a junção dos dois.

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
        vetor_direito[j] = v[meio + 1 + j];
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
}

```

A função abaixo, junta os vetores locais (que já estão ordenados), para formar um array global. 

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

Por fim, na main a primeira função a ser cumprida é o recebimento dos arquivos de entrada que é feito através dos parâmetros recebidos, argc e argv, para tanto é criado um laço de repetição, dentro deste é feita a leitura dos dados de cada um dos arquivos de entrada, os mesmos são posteriormente salvos em um vetor. Caso o usuário não tenha fornecido a quantidade mínima de argumentos para o programa funcionar, é emitida uma mensagem de erro na tela "Faltam argumentos". Além disso, inicia-se uma contagem do tempo de execução do programa no início desta função, para tanto utiliza-se a função  gettimeofday() para pegar a configuração de tempo da máquina, em seguida utiliza-se a função clock() para pegar a contagem que está no clock da máquina, isto é o horário de máquina, sem configurações. Dessa forma, a função clock é utilizada para calcular o tempo estimado e a função gettimeofday para formatar esse tempo em segundos. Após isto, o programa salva em uma variável global o número de threads requisitada pelo usuário e cria um vetor para armazenar as threads a serem criadas. O próximo passo é dividir o total de números lidos pela quantidade de threads a serem criadas, e então cria-las e iniciá-las com a função thread_merge_sort e então usar a função “join” para anexá-las ao fluxo inicial. Por fim os valores ordenados são mesclados em um vetor principal que é usado para gravar os dados em um arquivo de saída.

```c
int main(int argc, const char * argv[]) {

    struct timeval  start, end;
    double time_spent;
    clock_t ctTempoInicial, ctTempoFinal;
   //ARQUIVO DE SAIDA
    FILE *fp;
    if(argc<2){
        printf("faltam argumentos /n");
    }
    else{
        NUM_THREADS = strtol (argv[1],NULL,10);/*transforma o valor em inteiro */

        pthread_t threads[NUM_THREADS];
        gettimeofday(&start, NULL);
        //pegar dos arquivos aqui
        for(int a=2; a<(argc-2);a++){
			printf("argv %s\n",argv[a]);
			FILE *fp = fopen(argv[a],"r");
				while(!feof(fp)){
				fscanf(fp,"%d\n",&v[TAMANHO++]);
				}
			fclose(fp);
		}

	NUMBERS_PER_THREAD = TAMANHO / NUM_THREADS; /*Divide a quantidade de números que cada thread deverá cuidar*/
        OFFSET = TAMANHO % NUM_THREADS; /*Verifica se nenhuma thread está com números a mais*/

        ctTempoInicial = clock(); /*Inicia a contagem do tempo de execução*/
        for (long i = 0; i < NUM_THREADS; i ++) {
            int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
            if (rc){
                printf("ERRO\n");
                exit(-1);
            }
        }
	/*Junta as threads no fluxo princial*/
        for(long i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        mesclando_subvetores(v, NUM_THREADS, 1);/*mesclando todos os sub-vetores*/
        ctTempoFinal = clock();/*Finaliza a contagem do tempo de execução*/

        
        printf("Tempo Decorrido : %f\n", (double)(ctTempoFinal - ctTempoInicial) / CLOCKS_PER_SEC);
	
	fp = fopen(argv[argc-1],"w");
	for (int i = 0; i < TAMANHO; i ++) 
            fprintf(fp,"%d ",v[i]);
		//fclose(fp);
    }

	pthread_exit(NULL);

    /* teste para garantir que o vetor está ordenado*/
    /* teste_vetor_ordenado(v); */
    return 0;
}
```



### Compilação 

Para a compilação do programa, é necessário utilizar o prompt de comando de um computador com o sistema operacional Linux. Após abrir a tela do terminal, você deverá utilizar o comando cd <caminho do arquivo> até entrar no diretório do código. 

Em seguida, digite o comando:  gcc  mergesort.c  -lpthread  -o mergesort .

Logo após a compilação (isso pode levar alguns segundos), digite o comando ./mergesort <número de threads> <arquivos de entrada> -o <arquivo de saida> 

Exemplo: ./mergesort 2 arq1.dat  -o  saida.txt

![image-20191120182451560](C:\Users\isabe\AppData\Roaming\Typora\typora-user-images\image-20191120182451560.png)

Dessa forma, o programa executará a sua rotina, e criará o arquivo de saída no diretório no qual ele foi executado.

### Gráficos

O gráfico abaixo demonstra o desempenho do sistema em diferentes condições:

 **![img](https://lh3.googleusercontent.com/d9xxaQLmDO9H3cfXfYnpSmEKHy_fIk7XpRwqHvBvBUJtYozZcBXZgFcArgEBJstuo-ldpTgHB9zVIlwH_F_VrmryGfVUTagozt6mSSm9XbwsVGlB4huBw-9ky7Sk4dJM8KcTJpa1)** 

##### Tabela com valores do gráfico

![image-20191120181148575](C:\Users\isabe\AppData\Roaming\Typora\typora-user-images\image-20191120181148575.png)

#####  Conclusão

Dado os resultados apresentados no gráfico acima, é possível notar o comportamento de aumento no tempo de execução do programa em consonância com o aumento do número de arquivos e de threads, isso se dá devido a recursividade da função merge, que faz com que o programa passe mais tempo nela conforme a quantidade de threads aumenta. Ademais, o modelo de threads utilizado, que por sua vez separa igualmente o tempo para cada thread que foi criada (1 thread = 1 tempo), torna o tempo total uma soma cada vez maior conforme a quantidade de threads aumenta. 

### Vídeo

Segue o link com o funcionamento do programa:  [**https://youtu.be/Np9-gVg7Y0w**](https://youtu.be/Np9-gVg7Y0w) 	