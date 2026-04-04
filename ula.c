#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

int ula(int A, int B, int controle){
    int resultado=0;

    switch(controle){
        case 0: //add
            resultado = A+B;
        break;
        case 1: //sub
            resultado = A-B;
        break;
        case 2: //and
            resultado = A & B;
        break;
        case 3: //or
            resultado = A|B;
        break;
        default: 
            printf("Operacao nao implementada\n");
        break;
    }
    return resultado;
}