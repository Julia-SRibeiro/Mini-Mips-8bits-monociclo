#include <stdio.h>
#include <stdlib.h>

#define MAX 256
#define NUM_REG 8

// STRUCT / ENUM
typedef enum{

    type_i, type_r, type_j, type_outros

}classe_inst;

typedef struct{

    classe_inst type_inst;
    char inst_bin[17]; //string de 16 bits da instrução + '\0'
    int opcode, rs, rt, rd, funct, imm, addr;

}instrucao;

typedef struct{

    instrucao* inst; 
    int tamanho;

}memoria_instrucao;

typedef struct{

    int* dados; 
    int tamanho; 

}memoria_dados;

typedef struct {
    char reg[NUM_REG];
} banco_registradores;


// FUNCTIONS
void carrega_mem (memoria_instrucao* mem_inst);
void carrega_dat (memoria_dados* mem_dados);

void inicializa_reg(banco_registradores* banReg);


//MAIN
int main(){ 
    int menu;

    memoria_instrucao* mem_inst = (memoria_instrucao*) malloc(sizeof(memoria_instrucao));
    memoria_dados* mem_dados = (memoria_dados*) malloc(sizeof(memoria_dados));
    banco_registradores* banReg = malloc(sizeof(banco_registradores));

    mem_inst->inst = NULL;
    mem_dados->dados = NULL;

    //inicializa_reg(banReg);

    do {
        printf("\nMENU\n");
        printf("1. Carregar memória de instruções (.mem)\n");
        printf("2. Carregar memória de Dados (.dat)\n");
        printf("3. Imprimir memórias (instruções e dados)\n");
        printf("4. Imprimir banco de registradores\n");
        printf("5. Imprimir todo o simulador (registradores e memórias)\n");
        printf("6. Salvar .asm\n");
        printf("7. Salvar .dat\n");
        printf("8. Executa Programa (run)\n");
        printf("9. Executa uma instrução (Step)\n");
        printf("10. Volta uma instrução (Back)\n");
        printf("0. Sair\n");
        printf("Digite uma opção: ");
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
};


//FUNCTIONS
void carrega_mem (memoria_instrucao* mem_inst){

    FILE* arquivo = fopen("teste.mem", "r"); //abre arquivo no modo leitura    
    
    if(arquivo == NULL){
        printf("Erro: não foi possível ler o arquivo.");
        return;
    }

    if (mem_inst->inst != NULL) {
        free(mem_inst->inst);
    }

    mem_inst -> inst = (instrucao*) malloc(sizeof(instrucao) * MAX); //reserva bloco de memoria para as instruções 256 x tamanho do tipo instrução
    if (mem_inst->inst == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    mem_inst -> tamanho = 0;

    int valor_lido;

    while (fscanf (arquivo, "%d", &valor_lido) ==1 ){

        if (mem_inst -> tamanho >= MAX){
            printf("Memória cheia!");
            break;
        }

        mem_inst -> inst[mem_inst -> tamanho].opcode = valor_lido; //temporariamente guardado em opcode, implementar decodificacao da instrucao
        mem_inst -> tamanho++;
    }

    printf("%d instrucoes carregadas.\n", mem_inst->tamanho);
    fclose(arquivo);

};

void carrega_dat (memoria_dados* mem_dados){

    FILE* arquivo = fopen("teste.dat", "r"); //abre arquivo no modo leitura

    if(arquivo == NULL){
        printf("Erro: não foi possível ler o arquivo."); //verifica acesso ao arquivo
        return;   
    }

    if (mem_dados->dados != NULL) {
        free(mem_dados->dados);
    }

    mem_dados -> dados = (int*) malloc(sizeof(int) * MAX); //reserva um bloco de memória com tamanho 256 x 4 (bits de int) na  memoria dinâmica (heap)
    if (mem_dados->dados == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    mem_dados -> tamanho = 0; 

    int valor_lido;

    while (fscanf(arquivo, "%d", &valor_lido) == 1){

        if (mem_dados -> tamanho >= MAX){
            printf("Memória cheia!");
            break;
        }

        mem_dados -> dados[mem_dados -> tamanho] = valor_lido;

        mem_dados -> tamanho++; //incrementa quantidade de dados lidos em +1 

    }

    printf("%d dados carregados.\n", mem_dados->tamanho);
    fclose(arquivo);

};















void inicializa_reg(banco_registradores* banReg) {
    for (int i=0; i<NUM_REG; i++) {
    banReg->[i]=0;
    }
}


