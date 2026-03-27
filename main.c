#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sistema.h>

int main(){ 
    int menu;

    memoria_instrucao* mem_inst = (memoria_instrucao*) malloc(sizeof(memoria_instrucao));
    memoria_dados* mem_dados = (memoria_dados*) malloc(sizeof(memoria_dados));

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
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 0:
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

    return 0;
}