#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX 256

// STRUCTs e ENUMs
typedef enum {
    type_i, type_r, type_j, type_outros
} classe_inst;

typedef struct {
    classe_inst type_inst;
    char inst_bin[17];
    int opcode, rs, rt, rd, funct, imm, addr;
} instrucao;

typedef struct {
    instrucao* inst; 
    int tamanho;
} memoria_instrucao;

typedef struct {
    int* dados; 
    int tamanho; 
} memoria_dados;

// FUNCTIONS
void limpa_buffer();
void carrega_mem(memoria_instrucao* mem_inst);
void carrega_dat(memoria_dados* mem_dados);

void limpa_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}

#endif