#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void executa_programa(memoria_instrucao* mem_inst) {
    if (mem_inst ->inst == NULL || mem_inst -> tamanho == 0) {
printf("Nenhuma instrucao carregada.\n");
    }
    
int PC=0;

while(PC < (mem_inst->tamanho)) {
printf("PC = %d\n", PC);

// Não faz nada com a instrução ainda, depois de separar o opcode precisa alterar
    
PC++;
}
printf("Fim do programa.\n");
};