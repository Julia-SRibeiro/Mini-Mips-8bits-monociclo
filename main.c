#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

int main(){ 
    int menu;

    memoria_instrucao* mem_inst = (memoria_instrucao*) malloc(sizeof(memoria_instrucao));
    memoria_dados* mem_dados = (memoria_dados*) malloc(sizeof(memoria_dados));
    banco_registradores* banReg = malloc(sizeof(banco_registradores));
    
    inicializa_reg(banReg);

    mem_inst->inst = NULL;
    mem_dados->dados = NULL;

    do {
        printf("\nMENU\n");
        printf("1. Carregar memoria de instrucoes (.mem)\n");
        printf("2. Carregar memoria de Dados (.dat)\n");
        printf("3. Imprimir memorias (instrucoes e dados)\n");
        printf("4. Imprimir banco de registradores\n");
        printf("5. Imprimir todo o simulador (registradores e memorias)\n");
        printf("6. Salvar .asm\n");
        printf("7. Salvar .dat\n");
        printf("8. Executa Programa (run)\n");
        printf("9. Executa uma instrucao (Step)\n");
        printf("10. Volta uma instrucao (Back)\n");
        printf("0. Sair\n");
        printf("Digite uma opcao: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                carrega_mem (mem_inst);
            break;
            case 2:
                carrega_dat (mem_dados);
            break;
            case 3:
                print_mem_inst(mem_inst);
                print_mem_dat(mem_dados);
            break;
            case 4:
                print_banco_reg(banReg);
            break;
            case 5:
                print_complete(mem_inst, mem_dados, banReg);
            break;
            case 6:
            // .asm
            break;
            case 7:
                salva_dat(mem_dados);
            break;
            case 8:
                executa_programa(mem_inst, banReg, mem_dados);
            break;
            case 9:
            // step
            break;
            case 10:
            //back step
            break;
            case 0:
                printf("Encerrando programa.");
            break;
            default:
                printf("Numero invalido!");
                break;
        }
    } while (menu !=0);

    free(mem_inst->inst);
    free(mem_inst);
    free(mem_dados->dados);
    free(mem_dados);
    free(banReg);

    return 0;
}

// FUNCTIONS
void limpa_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int separa_bits(char *b, int ini, int nBits) {
    int n = 0;
    for (int i = 0; i < nBits; i++) {
        n = (n << 1) | (b[ini + i] == '1' ? 1 : 0); 
        //n = (n * 2) + (b[ini + i] == '1' ? 1 : 0);
    }
    return n;
}
int bits_imm(char *b) {
    int val = separa_bits(b, 10, 6);
    return (val << 2) >> 2;
}
int bits_jump(char *b) {
    int val = separa_bits(b, 4, 12);
    return val & 0xFF;
}

void print_complete(memoria_instrucao* mem_inst, memoria_dados* mem_dados, banco_registradores* banReg){
 printf("\n=========== ESTADO DO SIMULADOR ===========\n");

    // Memória de instruções
    printf("\n--- Memoria de Instrucoes ---\n");
    print_mem_inst(mem_inst);
    printf("\n");

    // Memória de dados
    printf("\n--- Memoria de Dados ---\n");
    print_mem_dat(mem_dados);
    printf("\n");

    // Banco de registradores
    printf("\n--- Banco de Registradores ---\n");
    print_banco_reg(banReg);

}

void executa_programa(memoria_instrucao* mem_inst, banco_registradores* banReg, memoria_dados* mem_dados) {
        if (mem_inst ->inst == NULL || mem_inst -> tamanho == 0) {
    printf("Nenhuma instrucao carregada.\n");
            return;
        }
        
    int PC=0;

    while(PC < (mem_inst->tamanho)) {
    printf("PC = %d\n", PC);

    instrucao *inst = &mem_inst->inst[PC];
    sinais s = decoder(inst);
    int A = banReg->reg[inst->rs];
    int B;

    if(s.ula_fonte == 1) //verifica se vai usar imediato
        B = inst->imm;
    else
        B = banReg->reg[inst->rt];
        
    int resultado = ula(A, B, s.controle_ula); //chama a ula

    if(s.esc_mem == 1){ //SW
        if(resultado >= 0 && resultado < MAX)
        mem_dados->dados[resultado] = banReg->reg[inst->rt];
        
        else
            printf("Endereco invalido: %d\n", resultado);
    }

    if(s.mem_para_reg == 1){ //LW
        if(resultado >=0 && resultado < MAX)
            banReg->reg[inst->rt] = mem_dados->dados[resultado];
            
        else
            printf("Endereco invalido: %d\n", resultado);
    }


    else if(s.esc_reg == 1){  //R e ADDI
        int destino;

        if(s.reg_destino == 1)
            destino = inst->rd;
        else
            destino = inst->rt;
        
        banReg->reg[destino] = resultado;
    }

    if(s.branch == 1 && resultado == 0){ //BEQ
        PC = PC + inst->imm;
        continue;
    }
    if(s.jump == 1){ //JUMP
        PC = inst->addr;
        continue;
    }
        
    PC++;
    }
    printf("Fim do programa.\n");
};