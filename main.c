#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"


int main(){ 
    int menu;

    CPU *cpu = (CPU*) malloc(sizeof(CPU));
    cpu->mem_inst = (memoria_instrucao*) malloc(sizeof(memoria_instrucao));
    cpu->mem_dados = (memoria_dados*) malloc(sizeof(memoria_dados));
    cpu->banco_regs = (banco_registradores*) malloc(sizeof(banco_registradores));
    cpu->historico = (salva_estado*) malloc(sizeof(salva_estado) * MAX_MEM);
    
    cpu->mem_inst->inst = NULL;
    cpu->mem_dados->dados = NULL;
    cpu->pc = cpu->ciclos = cpu->i_hist = 0;
    inicializa_reg(cpu);

    do {
        printf("\nMENU\n"
            "1. Carregar memoria de instrucoes (.mem)\n"
            "2. Carregar memoria de Dados (.dat)\n"
            "3. Executa Programa (Run)\n"
            "4. Executa uma instrucao (Step)\n"
            "5. Volta uma instrucao (Back Step) \n"
            "6. Ressetar programa (Reset)\n"
            "7. Imprimir memorias (instrucoes e dados)\n"
            "8. Imprimir banco de registradores\n"
            "9. Imprimir todo o simulador (registradores e memorias)\n"
            "10. Imprimir estatisticas de execucao\n"
            "11. Salvar .dat\n"
            "12. Salvar .asm\n"
            "0. Sair\n"
            "Digite uma opcao: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                carrega_mem (cpu);
            break;
            case 2:
                carrega_dat (cpu);
            break;
            case 3:
                executa_programa(cpu);
            break;
            case 4:
                if(cpu->ciclos == 0){
                    cpu->est = (estatisticas){0};
                }
                executa_instrucao(cpu);
            break;
            case 5:
                volta_instrucao(cpu);
            break;
            case 6:
                // Reset
            break;
            case 7:
                print_mem_inst(cpu);
                print_mem_dat(cpu);
            break;
            case 8:
                print_banco_reg(cpu);
            break;
            case 9:
                print_complete(cpu);
                break;
            case 10:
                print_est(cpu);
            break;
            case 11:
                salva_dat(cpu);
            break;
            case 12:
                salva_asm(cpu);
            break;
            case 0:
                printf("Encerrando programa.\n");
            break;
            default:
                printf("Numero invalido!");
            break;
        }
    } while (menu !=0);

    free(cpu->mem_inst->inst);
    free(cpu->mem_inst);
    free(cpu->mem_dados->dados);
    free(cpu->mem_dados);
    free(cpu->banco_regs);
    free(cpu->historico); 
    free(cpu);

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

void print_complete(CPU *cpu){
    printf("\n=========== ESTADO DO SIMULADOR ===========\n");

    // Memória de instruções
    printf("\n--- Memoria de Instrucoes ---\n");
    print_mem_inst(cpu);
    printf("\n");

    // Memória de dados
    printf("\n--- Memoria de Dados ---\n");
    print_mem_dat(cpu);
    printf("\n");

    // Banco de registradores
    printf("\n--- Banco de Registradores ---\n");
    print_banco_reg(cpu);
}
void print_est(CPU *cpu) {
    printf("\n===== ESTATISTICAS =====\n");
    printf("\nTotal de instrucoes: %d\n", cpu->est.total_inst);
    printf("\nTipo R: %d\n", cpu->est.total_r);
    printf("  ADD: %d\n", cpu->est.add);
    printf("  SUB: %d\n", cpu->est.sub);
    printf("  AND: %d\n", cpu->est.and_op);
    printf("  OR : %d\n", cpu->est.or_op);  
    printf("\nTipo I: %d\n", cpu->est.total_i);
    printf("  ADDI: %d\n", cpu->est.addi);
    printf("  BEQ : %d\n", cpu->est.beq);
    printf("  LW  : %d\n", cpu->est.lw);
    printf("  SW  : %d\n", cpu->est.sw);
    printf("\nTipo J: %d\n", cpu->est.total_j);
    printf("  JUMP: %d\n", cpu->est.jump);
}

void salvar_estado(CPU *cpu) {
    if (cpu->i_hist < MAX_MEM) {
        cpu->historico[cpu->i_hist].pc = cpu->pc;
        memcpy(cpu->historico[cpu->i_hist].reg, cpu->banco_regs->reg, MAX_REG * sizeof(char));
        memcpy(cpu->historico[cpu->i_hist].dados, cpu->mem_dados->dados, MAX_MEM * sizeof(int));
        cpu->historico[cpu->i_hist].cont_r = cpu->cont_r;
        cpu->historico[cpu->i_hist].cont_i = cpu->cont_i;
        cpu->historico[cpu->i_hist].cont_j = cpu->cont_j;
        cpu->historico[cpu->i_hist].est = cpu->est;
        cpu->i_hist++;
    }
}
void executa_instrucao(CPU *cpu) {
    if (cpu->mem_inst->inst == NULL || cpu->mem_inst->tamanho == 0) {
        printf("Nenhuma instrucao carregada.\n");
        return;
    }

    instrucao *inst = &cpu->mem_inst->inst[cpu->pc];
    salvar_estado(cpu);
    sinais s = decoder(inst, cpu);
    int proximo_PC=0;
    printf("PC = %d\n", (cpu->pc));

    cpu->est.total_inst++;

    if (inst->opcode == 0) {
        cpu->est.total_r++;

        switch(inst->funct){
            case 0: cpu->est.add++; break;
            case 1: cpu->est.sub++; break;
            case 2: cpu->est.and_op++; break;
            case 3: cpu->est.or_op++; break;
        }
    }
    else if (inst->opcode == 4) {
        cpu->est.total_i++;
        cpu->est.addi++;
    }
    else if (inst->opcode == 8) {
        cpu->est.total_i++;
        cpu->est.beq++;
    }
    else if (inst->opcode == 11) {
        cpu->est.total_i++;
        cpu->est.lw++;
    }
    else if (inst->opcode == 15) {
        cpu->est.total_i++;
        cpu->est.sw++;
    }
    else if (inst->opcode == 2) {
        cpu->est.total_j++;
        cpu->est.jump++;
    }

    // Le registradores
    int dado_rs = (int)cpu->banco_regs->reg[inst->rs];
    int operando_B;
    if(s.ula_fonte == 1) {
        operando_B = inst->imm; // imediato
    }
    else {
        operando_B = (int)cpu->banco_regs->reg[inst->rt]; // registrador
    }

    // Realiza operacao
    int overflow, flag_zero;
    int resultado_ula = ula(dado_rs, operando_B, s.controle_ula, &overflow, &flag_zero);

    if(flag_zero == 1){
        printf("PC=%d | A=%d B=%d | Resultado=%d | Flag=%d \n", cpu->pc, dado_rs, operando_B, resultado_ula, flag_zero);
    }

    if (overflow == 1){
        printf("PC=%d | A=%d B=%d | Resultado=%d | Overflow=%d \n", cpu->pc, dado_rs, operando_B, resultado_ula, overflow);
    }

    // Atualiza PC
    if (s.jump == 1) { //Jump
        proximo_PC = inst->addr;
    } else if (s.branch == 1 && flag_zero == 1) { //BEQ
        proximo_PC = cpu->pc + 1 + inst->imm;
    } else {
        proximo_PC = cpu->pc + 1;

        // Acessa memoria
        int dado_lido_mem = 0;

        if (s.mem_para_reg == 1) { // LW
            if (resultado_ula >= 0 && resultado_ula < MAX_MEM) {
                dado_lido_mem = cpu->mem_dados->dados[resultado_ula];
            } else {
                printf("Endereco de memoria invalido (LW): %d\n", resultado_ula);
            }
        }
        if (s.esc_mem == 1) { // SW
            if (resultado_ula >= 0 && resultado_ula < MAX_MEM) {
                cpu->mem_dados->dados[resultado_ula] = operando_B;
            } else {
                printf("Endereco de memoria invalido (SW): %d\n", resultado_ula);
            }
        }
        if (s.esc_reg == 1) {// R e ADDI
            int reg_destino = (s.reg_destino == 1) ? inst->rd : inst->rt;
            int valor_final = (s.mem_para_reg == 1) ? dado_lido_mem : resultado_ula;
            cpu->banco_regs->reg[reg_destino] = (char)valor_final;
        }
    }
    cpu->pc = proximo_PC;
    cpu->ciclos++;
}
void volta_instrucao(CPU *cpu) {
    if(cpu->i_hist > 0) {
        cpu->i_hist--;
        cpu->ciclos--;

        cpu->pc = cpu->historico[cpu->i_hist].pc;
        memcpy(cpu->banco_regs->reg, cpu->historico[cpu->i_hist].reg, sizeof(cpu->historico[cpu->i_hist].reg));
        memcpy(cpu->mem_dados->dados, cpu->historico[cpu->i_hist].dados, sizeof(cpu->historico[cpu->i_hist].dados));
        cpu->cont_i = cpu->historico[cpu->i_hist].cont_i;
        cpu->cont_r = cpu->historico[cpu->i_hist].cont_r;
        cpu->cont_j = cpu->historico[cpu->i_hist].cont_j;
        cpu->est = cpu->historico[cpu->i_hist].est;
    
        printf("Instrucao desfeita\n"
            "PC = %d\n", cpu->pc);
    } else {
        printf("Nao ha instrucoes anteriores. \n");
    }
} 

void executa_programa(CPU *cpu) {
    if (cpu->mem_inst->inst == NULL || cpu->mem_inst->tamanho == 0) {
        printf("Nenhuma instrucao carregada.\n");
        return;
    }
    while(cpu->ciclos < MAX_MEM) {
        executa_instrucao(cpu);
    }
    printf("\nExecucao finalizada com sucesso em %d ciclos.\n", cpu->ciclos);
    print_est(cpu);
};