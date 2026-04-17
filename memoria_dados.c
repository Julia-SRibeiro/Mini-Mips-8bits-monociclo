#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void carrega_dat (CPU *cpu){
    char arq[50];

    printf("Nome do arquivo .dat: ");
    limpa_buffer();
    scanf("%s", arq);

    strcat(arq, ".dat");

    // Abre arquivo no modo leitura
    FILE* arquivo = fopen(arq, "r");
    if(arquivo == NULL){
        printf("Erro: nao foi possivel abrir o arquivo.");
        return;
    }

    // Zera memoria de dados
    if (cpu->mem_dados->dados != NULL) {
        free(cpu->mem_dados->dados);
    }

    // Alocacao de memoria (256 x tamanho do tipo instrucao)
    cpu->mem_dados->dados = (int*) malloc(sizeof(int) * MAX_MEM);
    if (cpu->mem_dados->dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Verifica arquivo.dat e salva na memoria de dados
    cpu->mem_dados->tamanho = 0;
    int valor, linha=0;
    while (fscanf(arquivo, "%d", &valor) == 1){
        linha++;
        if (valor < -128 || valor > 127) {
            printf("Linha %d ignorada. Valor %d fora de [-128, 127].\n", linha, valor);
            printf("%d: %d\n", linha, valor);
            cpu->mem_dados->tamanho++;
            continue;
        }
        if (cpu->mem_dados->tamanho >= MAX_MEM) {
            printf("Memoria cheia!");
            break;
        }

        cpu->mem_dados->dados[cpu->mem_dados->tamanho] = valor;
        cpu->mem_dados->tamanho++;
    }

    printf("%d dados carregados.\n", cpu->mem_dados->tamanho);
    fclose(arquivo);
}

void inicializa_dat(CPU *cpu){
    
        cpu->mem_dados->dados = (int*) malloc(sizeof(int) * MAX_MEM);
        if (cpu->mem_dados->dados == NULL) return;
        
        for(int i = 0; i < MAX_MEM; i++){
            cpu->mem_dados->dados[i] = 0;
       }
       cpu->mem_dados->tamanho = MAX_MEM;
}

void print_mem_dat(CPU *cpu){

   int total = cpu->mem_dados->tamanho;

   printf("\nMemoria de dados (%d posicao(oes) carregada(s)):\n", total);

   printf("+------+--------+\n");
   printf("| Addr |  Valor |\n");
   printf("+------+--------+\n");

    for (int i = 0; i < total; i++) {
        int d = cpu->mem_dados->dados[i];
        printf("| %4d | %6d |\n", i, d);
    }
    printf("+------+--------+\n");
    printf("Total: %d dados\n", MAX_MEM);
}

void salva_dat(CPU *cpu) {
    if (cpu->mem_dados->dados == NULL) {
        printf("Erro: Memoria de dados nao carregada.\n");
        return;
    }

    char arq[50];
    printf("Nome do arquivo de saida .dat: ");
    limpa_buffer();
    scanf("%s", arq);

    strcat(arq, ".dat");

    FILE* arquivo = fopen(arq, "w");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel criar o arquivo '%s'.\n", arq);
        return;
    }

    // Salva cada valor da memória em uma linha
    for (int i = 0; i < cpu->mem_dados->tamanho; i++) {
        fprintf(arquivo, "%d\n", cpu->mem_dados->dados[i]);
    }

    fclose(arquivo);
    printf("Memoria de dados salva em '%s' (%d posicoes).\n", arq, cpu->mem_dados->tamanho);
}