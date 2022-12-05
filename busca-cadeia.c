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
long int line_ocorrencia[MAX_OCORRENCIAS];
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
        file_sequences = fopen(argv[2], "r"); // Arquivo 2 contendo todas a sequencias
    }

    else {
        file_sequences = fopen(arq2, "r");
        printf("Digite o nome do arquivo 1(seq a ser buscada): ");
        scanf("%s", &arq1);
        printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas):");
        scanf("%s", &arq2);
        file_target = fopen(arq1, "r");
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

    fseek(file_sequences, 0L, SEEK_END); // Leva ponteiro do arquivo para o final
    arq2_size = ftell(file_sequences);   // O final representa o tamanho do arquivo
    fseek(file_sequences, 0L, SEEK_SET); // Leva o ponteiro do arquivo para o comeco

    line = 1;
    fgets(sequencia_busca, SIZE, file_target); // Guarda a primeira linha em sequencia_busca
    i_seq = 0; 
    ocorrencias = 0;                                // Representa o total de caracteres lidos ate o momento
    while (i_seq < arq2_size) {
        fgets(buff, SIZE, file_sequences); // Leitura de uma linha
        i_seq += strlen(buff);
        achou = strcmp(sequencia_busca, buff); // encontrou: achou=0

        if (achou == 0) {
            line_ocorrencia[ocorrencias] = line;
            ocorrencias++;
        }

        line++;
    }

    // Fecha os arquivos
    fclose(file_target);
    fclose(file_sequences);

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    printf("=======================================\n");
    printf("Total de ocorrencias = %d\n", ocorrencias);
    printf("Total de sequencias = %d\n", line - 1);
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
