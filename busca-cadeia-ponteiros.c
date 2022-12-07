// busca de sequencia genomica do arquivo1 no arquivo2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define SIZE 100
#define NOME_ARQ_SIZE 50
#define TRUE 1
#define FALSE 0
#define MAX_OCORRENCIAS 8000000

#define NUM_THREADS 16

long int line_ocorrencia[MAX_OCORRENCIAS];
long int thread_line_ocorrencias[NUM_THREADS][MAX_OCORRENCIAS];
long int thread_lines[NUM_THREADS];
long int thread_ocorrencias[NUM_THREADS];
long int thread_ocorrencias[NUM_THREADS];
int main(int argc, char *argv[])
{
    char buff[SIZE];
    char sequencia_busca[SIZE];
    char arq1[NOME_ARQ_SIZE];
    char arq2[NOME_ARQ_SIZE];
    long int ocorrencias;
    long int i, j;
    long int i_seq;
    int achou;
    FILE *file_target;
    FILE *file_sequences;
    long int arq2_size;
    long int line;

    TIMER_CLEAR;

    // Abrir o arquivo e pegar a palavra
    if (argc == 3) {
        file_target = fopen(argv[1], "r");    // Arquivo 1 contendo a sequencia a ser buscada
        file_sequences = fopen(argv[2], "r"); // Arquivo 2 contendo todas as sequencia
    }

    else {
         printf("Digite o nome do arquivo 1(seq a ser buscada): ");
         scanf("%s", &arq1);
         printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas):");
         scanf("%s", &arq2);
         file_target = fopen(arq1, "r");
         file_sequences = fopen(arq2, "r");
    }

    // Verificacao do nome do arquivo
    if (file_target == NULL) {
        printf("Arquivo 1  nao encontrado\n");
        exit(1);
    }
    if (file_sequences == NULL) {
        printf("Arquivo 2  nao encontrado\n");
        exit(1);
    }

    // Leitura do arquivo 1 linha por linha e busca pela sequencia lida
    // Leitura do arquivo 2 linha por linha ao buscar a sequencia lida do arquivo 1

    printf("INICIO\n");
    fflush(stdout);

    // Logica para encontrar o target dentro do arquivo
    TIMER_START;

    fgets(sequencia_busca, SIZE, file_target); // Guarda a primeira linha em sequencia_busca
    ocorrencias = 0; // Representa o total de caracteres lidos ate o momento
    line = 0;

    #pragma omp parallel num_threads(NUM_THREADS) private(buff, achou, i_seq, file_sequences) shared(thread_line_ocorrencias, thread_lines, thread_ocorrencias) firstprivate(line)
    {   
        i_seq = 0;

        if (argc == 3) {
            file_sequences = fopen(argv[2], "r");
        }
        else {
            file_sequences = fopen(arq2, "r");
        }

        fseek(file_sequences, 0L, SEEK_END); // Leva ponteiro do arquivo para o final
        arq2_size = ftell(file_sequences);   // O final representa o tamanho do arquivo
        fseek(file_sequences, 0L, SEEK_SET); // Leva o ponteiro do arquivo para o comeco

        int nthreads = omp_get_num_threads();
        int ithread = omp_get_thread_num();

        long int size = arq2_size / nthreads;
        long int start = size * ithread;

        thread_lines[ithread] = line; // Inicializa contador de linhas da thread

        if (start != 0L) {
            fseek(file_sequences, start - 1L, SEEK_SET); // Leva o ponteiro do arquivo para uma posicao anterior do comeco da thread
            fgets(buff, SIZE, file_sequences); // Leitura da linha que começa na posicao anterior do comeco da thread
            
            if (buff[0] == '\n') { // Se a linha anterior comeca com end line, a thread comeca no inicio de uma linha nova, entao essa linha sera considerada
                fseek(file_sequences, start, SEEK_SET); // Leva o ponteiro do arquivo para o comeco da thread
            }
            else {
                i_seq += strlen(buff);
            }
        }
        else {
            fseek(file_sequences, start, SEEK_SET); // Leva o ponteiro do arquivo para o comeco da thread
        }

        while (i_seq < size) {
            thread_lines[ithread]++;

            fgets(buff, SIZE, file_sequences); // Leitura de uma linha
            i_seq += strlen(buff);
        
            achou = strcmp(sequencia_busca, buff); // encontrou: achou=0

            if (achou == 0) {
                thread_line_ocorrencias[ithread][thread_ocorrencias[ithread]] = thread_lines[ithread];
                thread_ocorrencias[ithread]++;                   
            } 
        }

        fclose(file_sequences);
    }
    fclose(file_target);

    // Arrumar as linhas das ocorrencias
    ocorrencias = 0; // Iterador do total de ocorrencias iterado ate o momento
    line = 0; // Contador do total de linhas de threads, cumulativo a cada final de iteracao de thread
    for (int ithread = 0; ithread < NUM_THREADS; ithread++) {
        for (int iocorrencia = 0; iocorrencia < thread_ocorrencias[ithread]; iocorrencia++) {
            // Coloca no vetor de ocorrencias as ocorrencias da thread ithread somando as linhas locais da thread com a contagem de linhas correta
            line_ocorrencia[ocorrencias] = thread_line_ocorrencias[ithread][iocorrencia] + line;
            ocorrencias++; // Soma um no iterador de ocorrencias total
        }
        line += thread_lines[ithread]; // Acumula no contador de linhas o total de linhas da thread iterada
    }

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    printf("=======================================\n");
    printf("Total de ocorrencias = %d\n", ocorrencias);
    printf("Total de sequencias = %d\n", line);
    if (ocorrencias < 20) {
        for (i = 0; i < ocorrencias; i++)
            printf("sequencia %d  ", line_ocorrencia[i]);
        printf("\n");
    }
    else {
        for (i = 0; i < 5; i++)
            printf("sequencia %d  ", line_ocorrencia[i]);
        printf("\n");
        for (i = ocorrencias / 2; i < ocorrencias / 2 + 5; i++)
            printf("sequencia %d  ", line_ocorrencia[i]);
        printf("\n");
        for (i = ocorrencias - 5; i < ocorrencias; i++)
            printf("sequencia %d  ", line_ocorrencia[i]);
        printf("\n");
    }
    printf("=======================================\n");

    return 0;
}
