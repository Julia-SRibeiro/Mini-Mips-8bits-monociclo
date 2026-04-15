#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

int ula(int A, int B, int controle, int *overflow, int *flag_Zero){
    int resultado=0;
    *overflow=0;
    *flag_Zero=0;

    switch(controle){
        case 0: //add
            resultado = A + B;
            if ((A>0 && B>0 && resultado <0) || (A<0 && B<0 && resultado >0)){
                *overflow = 1;
            }
        break;
        case 1: //sub
            resultado = A - B;
            if((A>0 && B<0 && resultado <0) || (A<0 && B>0 && resultado >0)){
                *overflow=1; 
            }
        break;
        case 2: //and
            resultado = A & B;
        break;
        case 3: //or
            resultado = A | B;
        break;
        default: 
            printf("Operacao nao implementada\n");
        break;
    }
        if(resultado==0){
             *flag_Zero=1;
    }
    return resultado;
}