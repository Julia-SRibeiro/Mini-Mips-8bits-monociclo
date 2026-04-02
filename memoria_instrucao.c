#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void carrega_mem (memoria_instrucao* mem_inst){
    char arq[50];

    printf("Nome do arquivo .mem: ");
    limpa_buffer();
    scanf("%s", arq);

    // Abre arquivo no modo leitura
    FILE* arquivo = fopen(arq, "r"); 
    if(arquivo == NULL){
        printf("Erro: nao foi possivel abrir o arquivo.");
        return;
    }

    if (mem_inst->inst != NULL) {
        free(mem_inst->inst);
    }

    // Alocacao de memoria (256 x tamanho do tipo instrucao)
    mem_inst -> inst = (instrucao*) malloc(sizeof(instrucao) * MAX);
    if (mem_inst->inst == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Verifica arquivo.mem e salva na memoria de instrucoes
    mem_inst -> tamanho = 0;
    char bits[20];
    int linha = 0;
    while (fscanf (arquivo, "%s", bits) == 1){
        linha++;

        int len = strlen(bits);
        if (len != 16) {
            printf("Linha %d ignorada. Tamanho %d != 16\n", linha, len);
            printf("%d: %s\n", linha, bits);
            continue;
        }
        
        int valido = 1;
        for (int i = 0; i < 16; i++) {
            if (bits[i] != '0' && bits[i] != '1') {
                valido = 0;
                break;
            }
        }
        if (valido == 0) {
            printf("Linha %d ignorada. Caracter invalido %s\n", linha);
            printf("%d: %s", linha, bits);
            continue;
        }
    
        if (mem_inst -> tamanho >= MAX){
            printf("Memoria cheia!");
            break;
        }

        strcpy(mem_inst->inst[mem_inst->tamanho].inst_bin, bits);
        mem_inst -> tamanho++;
    }

    printf("%d instrucoes carregadas.\n", mem_inst->tamanho);
    fclose(arquivo);
}

void print_mem_inst(memoria_instrucao* imp_inst){
   printf("\n+-----+------------------+\n");
   printf("| PC  | Binario          |");
   printf("\n+-----+------------------+\n");


   for (int i = 0; i < imp_inst -> tamanho; i++){
       instrucao *inst = &imp_inst->inst[i];
       printf("| %3d | %-16s |\n", i, inst -> inst_bin);
   }
   printf("+-----+------------------+\n");
   printf("Total: %d instrucoes\n", imp_inst->tamanho);
}