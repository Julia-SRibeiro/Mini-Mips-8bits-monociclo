#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void inicializa_reg(banco_registradores* banReg) {
    for (int i=0; i<NUM_REG; i++) {
    banReg->reg[i]=0;
    }

    banReg->NOME_REG[0] = "R0";
    banReg->NOME_REG[1] = "R1";
    banReg->NOME_REG[2] = "R2";
    banReg->NOME_REG[3] = "R3";
    banReg->NOME_REG[4] = "R4";
    banReg->NOME_REG[5] = "R5";
    banReg->NOME_REG[6] = "R6";
    banReg->NOME_REG[7] = "R7";
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