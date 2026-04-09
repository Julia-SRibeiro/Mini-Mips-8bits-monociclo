#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void inicializa_reg(banco_registradores* banReg) {
    for (int i=0; i<NUM_REG; i++) {
    banReg->reg[i]=0;
    }

    banReg->NOME_REG[0] = "$t0";
    banReg->NOME_REG[1] = "$t1";
    banReg->NOME_REG[2] = "$t2";
    banReg->NOME_REG[3] = "$t3";
    banReg->NOME_REG[4] = "$t4";
    banReg->NOME_REG[5] = "$t5";
    banReg->NOME_REG[6] = "$t6";
    banReg->NOME_REG[7] = "$t7";
}

void print_banco_reg(banco_registradores* banReg){
        if (banReg == NULL) {
        printf("Banco de registradores nao inicializado.\n");
        return;
    }

    printf("\nBanco de Registradores:\n");
    printf("+------+--------+\n");
    printf("| Reg  |  Valor |\n");
    printf("+------+--------+\n");

    for (int i = 0; i < NUM_REG; i++) {
        printf("| %4s | %6d |\n", banReg->NOME_REG[i], banReg->reg[i]);
    }

    printf("+------+--------+\n");
}