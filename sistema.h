#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_MEM 256
#define MAX_REG 8

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
    instrucao *inst; 
    int tamanho;
} memoria_instrucao;
typedef struct {
    int *dados;
    int tamanho;
} memoria_dados;
typedef struct {
    char reg[MAX_REG];
    char *NOME_REG[MAX_REG];
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

typedef struct {
  int pc;
  char reg[MAX_REG];
  int dados[MAX_MEM];
  int cont_r, cont_i, cont_j, cont_total;
} salva_estado;

typedef struct {
    int pc;
    memoria_instrucao *mem_inst;
    memoria_dados *mem_dados;
    banco_registradores *banco_regs;
    salva_estado *historico;
    int cont_r, cont_i, cont_j, cont_total;
    int i_hist;
    int ciclos;
} CPU;

typedef struct {
    int total_inst, add, sub, and_op, or_op;
    int addi, sw, lw, beq, jump;
    int total_r, total_i, total_j;
} estatisticas;


// FUNCTIONS
void limpa_buffer();
void carrega_mem(CPU *cpu);
void print_mem_inst(CPU *cpu);
void carrega_dat(CPU *cpu);
void print_mem_dat(CPU *cpu);
void salva_dat(CPU *cpu);
void inicializa_reg(CPU *cpu);
void print_banco_reg(CPU *cpu);
void print_complete(CPU *cpu);
int separa_bits(char *b, int ini, int nBits);
int bits_imm(char *b, int ini, int nBits);
int bits_jump(char *b);
sinais decoder(instrucao *inst);
int ula(int A, int B, int controle, int* overflow,  int* flag_zero);
void salvar_estado(CPU *cpu);
void executa_instrucao(CPU *cpu);
void volta_instrucao(CPU *cpu);
void executa_programa(CPU *cpu);
void disassembla(instrucao *inst, char *buffer);
void salva_asm(CPU *cpu);

#endif