#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void carrega_mem (CPU *cpu){
    char arq[50];

    printf("Nome do arquivo .mem: ");
    limpa_buffer();
    scanf("%s", arq);

    strcat(arq, ".mem");

    // Abre arquivo no modo leitura
    FILE* arquivo = fopen(arq, "r"); 
    if(arquivo == NULL){
        printf("Erro: nao foi possivel abrir o arquivo.");
        return;
    }

    if (cpu->mem_inst->inst != NULL) {
        free(cpu->mem_inst->inst);
    }

    // Alocacao de memoria (256 x tamanho do tipo instrucao)
    cpu->mem_inst->inst = (instrucao*) malloc(sizeof(instrucao) * MAX_MEM);
    if (cpu->mem_inst->inst == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Verifica arquivo.mem e salva na memoria de instrucoes
    cpu->mem_inst->tamanho = 0;
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
    
        if (cpu->mem_inst -> tamanho >= MAX_MEM){
            printf("Memoria cheia!");
            break;
        }

        strcpy(cpu->mem_inst->inst[cpu->mem_inst->tamanho].inst_bin, bits);
        cpu->mem_inst->tamanho++;
        
    }
    if(cpu->mem_inst->tamanho > 0){
        printf("Instruções carregadas com sucesso para a Memória de Instruções!\n");
    }
    fclose(arquivo);
}

void print_mem_inst(CPU *cpu){

    if (cpu->mem_inst->inst == NULL || cpu->mem_inst->tamanho == 0) {
        printf("Nenhuma instrucao carregada.\n");
        return;
    }

    printf("\n+------------------------------------------------------+\n");
    printf("|                 Memória de Instruções                |");
    printf("\n+------+------------------+----------------------------+\n");
    printf("| Addr | Binario          |          Assembly          |");
    printf("\n+------+------------------+----------------------------+\n");

   
    for (int i = 0; i < MAX_MEM; i++){
        instrucao *inst = &cpu->mem_inst->inst[i];
        printf("| %4d | %-16s |", i, inst -> inst_bin);
        if(i < cpu->mem_inst->tamanho){
            int pc_original = cpu->pc;
            cpu->pc = i;
            print_asm(cpu);
            printf("|\n");
            cpu->pc = pc_original;
        }else{
            printf("%-28s|\n", "");
        }
    }
    printf("+------+------------------+----------------------------+\n");
}

void disassembla(instrucao *inst, char *buffer, CPU *cpu) {
    decoder(inst, cpu); 

    switch (inst->opcode) {
        case 0: // Tipo R
            switch (inst->funct) {
                case 0: sprintf(buffer, "add $r%d, $r%d, $r%d", inst->rd, inst->rs, inst->rt); break;
                case 1: sprintf(buffer, "sub $r%d, $r%d, $r%d", inst->rd, inst->rs, inst->rt); break;
                case 2: sprintf(buffer, "and $r%d, $r%d, $r%d", inst->rd, inst->rs, inst->rt); break;
                case 3: sprintf(buffer, "or  $r%d, $r%d, $r%d", inst->rd, inst->rs, inst->rt); break;
                default: sprintf(buffer, "Instrução desconhecida"); break;
            }
            break;
        case 2:  sprintf(buffer, "j %d", inst->addr); break;
        case 4:  sprintf(buffer, "addi $r%d, $r%d, %d", inst->rt, inst->rs, inst->imm); break;
        case 8:  sprintf(buffer, "beq $r%d, $r%d, %d", inst->rt, inst->rs, inst->imm); break;
        case 11: sprintf(buffer, "lw $r%d, %d($%d)", inst->rt, inst->imm, inst->rs); break;
        case 15: sprintf(buffer, "sw $r%d, %d($%d)", inst->rt, inst->imm, inst->rs); break;
        default: sprintf(buffer, "data %s", inst->inst_bin); break;
    }
}

void salva_asm(CPU *cpu) {
    if (cpu->mem_inst->inst == NULL || cpu->mem_inst->tamanho == 0) {
        printf("Memoria de instrucoes vazia.\n");
        return;
    }

    char arq[50];
    printf("Nome do arquivo de saida .asm: ");
    limpa_buffer();
    scanf("%s", arq);

    strcat(arq, ".asm");

    FILE *f = fopen(arq, "w");
    if (!f) {
        printf("Erro ao criar arquivo.\n");
        return;
    }
    for (int i = 0; i < cpu->mem_inst->tamanho; i++) {
        char linha_asm[64];
        disassembla(&cpu->mem_inst->inst[i], linha_asm, cpu);
        fprintf(f, "%s \n", linha_asm);
    }

    fclose(f);
    printf("Arquivo '%s' salvo com sucesso!\n", arq);
}

void print_asm(CPU *cpu){

    char inst_asm[64];
    disassembla(&cpu->mem_inst->inst[cpu->pc], inst_asm, cpu);
    printf("%-28s", inst_asm);

}