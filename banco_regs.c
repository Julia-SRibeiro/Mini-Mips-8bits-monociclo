#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void inicializa_reg(CPU *cpu) {
    for (int i=0; i<MAX_REG; i++) {
    cpu->banco_regs->reg[i]=0;
    }

    cpu->banco_regs->NOME_REG[0] = "$0";
    cpu->banco_regs->NOME_REG[1] = "$r1";
    cpu->banco_regs->NOME_REG[2] = "$r2";
    cpu->banco_regs->NOME_REG[3] = "$r3";
    cpu->banco_regs->NOME_REG[4] = "$r4";
    cpu->banco_regs->NOME_REG[5] = "$r5";
    cpu->banco_regs->NOME_REG[6] = "$r6";
    cpu->banco_regs->NOME_REG[7] = "$r7";
}

void print_banco_reg(CPU *cpu){
        if (cpu->banco_regs->reg == NULL) {
        printf("Banco de registradores nao inicializado.\n");
        return;
    }

    printf("\nBanco de Registradores:\n");
    printf("+------+--------+\n");
    printf("| Reg  |  Valor |\n");
    printf("+------+--------+\n");

    for (int i = 0; i < MAX_REG; i++) {
        printf("| %4s | %6d |\n", cpu->banco_regs->NOME_REG[i], cpu->banco_regs->reg[i]);
    }

    printf("+------+--------+\n");
}