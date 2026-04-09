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
                salva_asm(mem_inst, mem_dados);
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
int bits_imm(char *b, int ini, int nBits) {
    int val = separa_bits(b, ini, nBits);
    signed char sinal = (signed char)(val << 2);
    return (int)(sinal >> 2);
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
    int ciclos=0;

    while(PC >= 0 && PC < mem_inst-> tamanho && ciclos < CICLOS_MAX){
        instrucao *inst = &mem_inst->inst[PC];

        //decode
        sinais s = decoder(inst);
        printf("PC = %d\n", PC);
        //le registradores
        int dado_rs = (int)banReg->reg[inst->rs];
        int dado_rt = (int)banReg->reg[inst->rt];

        int overflow=0;
        int operando_B = (s.ula_fonte == 1) ? inst->imm : dado_rt;
        int resultado_ula = ula(dado_rs, operando_B, s.controle_ula, &overflow);

        if (overflow == 1){
            printf("PC=%d | A=%d B=%d | Resultado=%d | Overflow=%d \n", PC, dado_rs, operando_B, resultado_ula, overflow);
        }

        int dado_lido_mem = 0;

        if (s.esc_mem == 1) { // instrução SW
            if (resultado_ula >= 0 && resultado_ula < MAX) {
                mem_dados->dados[resultado_ula] = dado_rt;
            } else {
                printf("Endereco de memoria invalido (SW): %d\n", resultado_ula);
            }
        }

        if (s.mem_para_reg == 1) { //instrução LW
            if (resultado_ula >= 0 && resultado_ula < MAX) {
                dado_lido_mem = mem_dados->dados[resultado_ula];
            } else {
                printf("[ERRO] Endereco de memoria invalido (LW): %d\n", resultado_ula);
            }
        }

        //escrita no Banco de Registradores (Write Back)
        if (s.esc_reg == 1) {
            //define o destino: RD (Tipo R) ou RT (Tipo I)
            int reg_destino = (s.reg_destino == 1) ? inst->rd : inst->rt;
           
            //se for LW, o dado vem da memória. Caso contrário, vem da ULA.
            int valor_final = (s.mem_para_reg == 1) ? dado_lido_mem : resultado_ula;
           
            banReg->reg[reg_destino] = (char)valor_final;
        }

        //atualização do PC (Controle de Fluxo)
        int proximo_PC = PC + 1;

        //BEQ: Se os valores forem iguais, a subtração na ULA resulta em 0
        if (s.branch == 1 && resultado_ula == 0) {
            proximo_PC = PC + 1 + inst->imm;
        }

        //Jump
        if (s.jump == 1) {
            proximo_PC = inst->addr;
        }

        PC = proximo_PC;
        ciclos++;
    }

    if (ciclos >= CICLOS_MAX) {
        printf("\nExecucao interrompida pelo limite de ciclos (possivel loop infinito).\n");
    } else {
        printf("\nExecucao finalizada com sucesso em %d ciclos.\n", ciclos);
    }
};