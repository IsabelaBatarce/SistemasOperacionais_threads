#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

/* define variables for the problem */
int LENGTH = 0;
#define UPPER_LIM 50000
#define LOWER_LIM 1

/* define derived values from the variables */
int NUM_THREADS;
int arr[UPPER_LIM];
int NUMBERS_PER_THREAD;
int OFFSET;


/* function definitions */
void merge_sort(int arr[], int left, int right);
void merge(int arr[], int left, int middle, int right);
void* thread_merge_sort(void* arg);
void merge_partes(int arr[], int number, int aggregation);
void testa_ordem(int arr[]);

int main(int argc, const char * argv[]) {

    struct timeval  start, end;

    if(argc<2){
        printf("faltam argumentos \n");
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
				fscanf(fp,"%d\n",&arr[LENGTH++]);
				}
			fclose(fp);
		}

		NUMBERS_PER_THREAD = LENGTH / NUM_THREADS;
        OFFSET = LENGTH % NUM_THREADS;

        for (long i = 0; i < NUM_THREADS; i ++) {
            int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }

        for(int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        merge_partes(arr, NUM_THREADS, 1);


        gettimeofday(&end, NULL);

        printf("terminado");

        FILE * saida = fopen("saida.txt","w");
        for (int i = 0; i < LENGTH; i ++) {

            fprintf(saida,"%d ",arr[i]);
        }
        fclose(saida);
    }


    return 0;
}



void merge_partes(int arr[], int number, int aggregation) {
    for(int i = 0; i < number; i = i + 2) {
        int left = i * (NUMBERS_PER_THREAD * aggregation);
        int right = ((i + 2) * NUMBERS_PER_THREAD * aggregation) - 1;
        int middle = left + (NUMBERS_PER_THREAD * aggregation) - 1;
        if (right >= LENGTH) {
            right = LENGTH - 1;
        }
        merge(arr, left, middle, right);
    }
    if (number / 2 >= 1) {
        merge_partes(arr, number / 2, aggregation * 2);
    }
}

/** assigns work to each thread to perform merge sort */
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

/* test to ensure that the array is in sorted order */
void testa_ordem(int arr[]) {
    int max = 0;
    for (int i = 1; i < LENGTH; i ++) {
        if (arr[i] >= arr[i - 1]) {
            max = arr[i];
        } else {
            printf("Error. Out of order sequence: %d found\n", arr[i]);
            return;
        }
    }
    printf("Array is in sorted order\n");
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}

void merge(int arr[], int left, int middle, int right) {
    int i = 0;
    int j = 0;
    int k = 0;
    int left_length = middle - left + 1;
    int right_length = right - middle;
    int left_array[left_length];
    int right_array[right_length];


    for (int i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }


    for (int j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }

    i = 0;
    j = 0;

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
