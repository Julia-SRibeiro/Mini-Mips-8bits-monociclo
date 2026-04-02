#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX 256
#define NUM_REG 8
#define CICLOS_MAX (MAX * 20) /* limite anti-loop infinito */

// STRUCTs e ENUMs
typedef enum {
    type_r, type_j, type_i, type_outros
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

typedef struct {
    char reg[NUM_REG];
    char *NOME_REG[NUM_REG];
} banco_registradores;

typedef struct {
  int reg_destino; // 1=rd, 0=rt
  int ula_fonte; // 0=registrador, 1=imm
  int mem_para_reg; // 1=memoria, 0=ula
  int esc_reg; // 1=escreve registrador
  int esc_mem; // 1=escreve memoria
  int branch; // 1=BEQ      
  int jump; // 1=JUMP
  int controle_ula; // ULA_ADD/SUB/AND/OR
} sinais;

// FUNCTIONS
void limpa_buffer();
void carrega_mem(memoria_instrucao* mem_inst);
void print_mem_inst(memoria_instrucao* imp_inst);
void carrega_dat(memoria_dados* mem_dados);
void print_mem_dat(memoria_dados* imp_dados);
void salva_dat(memoria_dados* mem_dados);
void inicializa_reg(banco_registradores* banReg);
void print_banco_reg(banco_registradores* banReg);
void print_complete(memoria_instrucao* mem_inst, memoria_dados* mem_dados, banco_registradores* banReg);
int separa_bits(char *b, int ini, int nBits);
int bits_imm(char *b);
int bits_jump(char *b);
sinais decoder(instrucao *inst);
int ula(int A, int B, int controle);
void executa_programa(memoria_instrucao* mem_inst, banco_registradores* banReg, memoria_dados* mem_dados);
void disassembla(instrucao *inst, char *buffer, int size);
void salva_asm(memoria_instrucao* mem_inst, memoria_dados* mem_dados);

#endif