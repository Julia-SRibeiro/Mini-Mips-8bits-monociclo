#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

void inicializa_reg(banco_registradores* banReg) {
    for (int i=0; i<NUM_REG; i++) {
    banReg->reg[i]=0;
    }
}