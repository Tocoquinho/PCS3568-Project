//busca de sequencia genomica do arquivo1 no arquivo2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"

#define SIZE 100 
#define NOME_ARQ_SIZE   50
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
    FILE *f1;
    FILE *f2;
    long int arq2_size;
    long int line;

    TIMER_CLEAR;
//    TIMER_START;

    // Abrir o arquivo e pegar a palavra
    if (argc == 3){
    	f1 = fopen(argv[1], "r");
    	f2 = fopen(argv[2], "r");
    }

    else{
    	printf("Digite o nome do arquivo 1(seq a ser buscada): ");
    	scanf("%s",&arq1);
    	printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas):");
    	scanf("%s",&arq2);
    	f1 = fopen(arq1, "r");
    	f2 = fopen(arq2, "r");
    }

    // Verificacao do nome do arquivo
    if(f1 == NULL){
    	printf("Arquivo 1  nao encontrado\n");
    	exit(1);
    }
    if(f2 == NULL){
    	printf("Arquivo 2  nao encontrado\n");
    	exit(1);
    }


    // Leitura do arquivo 1 linha por linha e busca pela sequencia lida
    // Leitura do arquivo 2 linha por linha ao buscar a sequencia lida do arquivo 1
    TIMER_START;
printf("INICIO\n");
fflush(stdout);
    TIMER_START;
    fseek(f2,0L,SEEK_END);
    arq2_size=ftell(f2);
    fseek(f2,0L,SEEK_SET);

    line=1; 
    fgets(sequencia_busca,SIZE,f1);
    ocorrencias=0;
    i_seq=0;
    while(i_seq< arq2_size){
        fgets(buff, SIZE, f2);// Leitura de uma linha
        i_seq+=strlen(buff);
	achou=strcmp(sequencia_busca,buff); // encontrou: achou=0  
	if (achou == 0){
            line_ocorrencia[ocorrencias]=line;
	    ocorrencias++;
        } 
        line++;
    }
    line=line-1;
    // Fecha os arquivos
    fclose(f1);
    fclose(f2);

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    printf("=======================================\n");
    printf("Total de ocorrencias = %d\n",ocorrencias);
    printf("Total de sequencias = %d\n",line);
    if (ocorrencias <20) {
        for (i=0;i<ocorrencias;i++)
            printf("sequencia %d  ",line_ocorrencia[i]); 
        printf("\n");
    }
    else {
        for (i=0;i<5;i++)
            printf("sequencia %d  ",line_ocorrencia[i]); 
        printf("\n");
        for (i=ocorrencias/2;i<ocorrencias/2+5;i++)
            printf("sequencia %d  ",line_ocorrencia[i]); 
        printf("\n");
        for (i=ocorrencias-5;i<ocorrencias;i++)
            printf("sequencia %d  ",line_ocorrencia[i]); 
        printf("\n");
    }
    printf("=======================================\n");
    return 0;
    
}
