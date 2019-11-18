#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

/* definindo as variaveis  */
int TAMANHO = 0;
#define UPPER_LIM 10000
#define LOWER_LIM  1
int NUM_THREADS;
int v[UPPER_LIM];
int NUMBERS_PER_THREAD;
int OFFSET;


/* Declarando as funções */
void merge_sort(int v[], int inicio, int final);
void merge(int v[], int inicio, int meio, int final);
void* thread_merge_sort(void* arg);
void mesclando_subvetores(int v[], int numero, int aggregation);
void teste_vetor_ordenado(int v[]);

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

/* Mesclar seções dos vetores */
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

/** Atribui o trabalho para cada thread executar o merge sort */
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

/* Realizando o merge sort */
void merge_sort(int v[], int inicio, int final) {
    if (inicio < final) {
        int meio = inicio + (final - inicio) / 2;
        merge_sort(v, inicio, meio);
        merge_sort(v, meio + 1, final);
        merge(v, inicio, meio, final);
    }
}

/* Função merge */
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
