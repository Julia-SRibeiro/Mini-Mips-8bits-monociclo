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
            printf("Linha %d ignorada. Caracter invalido %s\n", linha, bits);
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

void disassembla(instrucao *inst, char *buffer, int size) {
    // Primeiro, rodamos o decoder para garantir que os campos rs, rt, rd, etc., estejam preenchidos
    decoder(inst); 

    switch (inst->opcode) {
        case 0: // Tipo R
            switch (inst->funct) {
                case 0: snprintf(buffer, size, "add $t%d, $t%d, $t%d", inst->rd, inst->rs, inst->rt); break;
                case 1: snprintf(buffer, size, "sub $t%d, $t%d, $t%d", inst->rd, inst->rs, inst->rt); break;
                case 2: snprintf(buffer, size, "and $t%d, $t%d, $t%d", inst->rd, inst->rs, inst->rt); break;
                case 3: snprintf(buffer, size, "or  $t%d, $t%d, $t%d", inst->rd, inst->rs, inst->rt); break;
                default: snprintf(buffer, size, "unknown R"); break;
            }
            break;
        case 2:  snprintf(buffer, size, "j %d", inst->addr); break;
        case 4:  snprintf(buffer, size, "addi $t%d, $t%d, %d", inst->rt, inst->rs, inst->imm); break;
        case 8:  snprintf(buffer, size, "beq $t%d, $t%d, %d", inst->rs, inst->rt, inst->imm); break;
        case 11: snprintf(buffer, size, "lw $t%d, %d($t%d)", inst->rt, inst->imm, inst->rs); break;
        case 15: snprintf(buffer, size, "sw $t%d, %d($t%d)", inst->rt, inst->imm, inst->rs); break;
        default: snprintf(buffer, size, "data %s", inst->inst_bin); break;
    }
}

void salva_asm(memoria_instrucao* mem_inst, memoria_dados* mem_dados) {
    if (mem_inst->inst == NULL || mem_inst->tamanho == 0) {
        printf("Erro: Memoria de instrucoes vazia.\n");
        return;
    }

    char arq[50];
    printf("Nome do arquivo de saida .asm: ");
    limpa_buffer();
    scanf("%s", arq);

    FILE *f = fopen(arq, "w");
    if (!f) {
        printf("Erro ao criar arquivo.\n");
        return;
    }

    for (int i = 0; i < mem_inst->tamanho; i++) {
        char linha_asm[64];
        disassembla(&mem_inst->inst[i], linha_asm, sizeof(linha_asm));
        fprintf(f, "    %s \n", linha_asm);
    }

    fclose(f);
    printf("Arquivo '%s' salvo com sucesso!\n", arq);
}