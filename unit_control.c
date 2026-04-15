#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"

sinais decoder(instrucao *inst, CPU *cpu) {
    char *b = inst->inst_bin;
    sinais s;
    inst->opcode = separa_bits(b, 0, 4);

    switch (inst->opcode) {
        case 0:
            // Tipo R
            inst->type_inst = 0;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->rd = separa_bits(b, 10, 3);
            inst->funct = separa_bits(b, 13, 3);
            inst->imm = inst->addr = 0;

            s.reg_destino = 1; // destino = rd
            s.ula_fonte = 0; // operacao com = rt
            s.mem_para_reg = 0;
            s.esc_reg = 1; // escreve no banco
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;

            switch(inst->funct){
                case 0: s.controle_ula = 0; break; //ADD
                case 1: s.controle_ula = 1; break; //SUB
                case 2: s.controle_ula = 2; break; //AND
                case 3: s.controle_ula = 3; break; //OR
                default: printf("Funct invalido\n"); break;
            }
        break;
        case 2: 
            // Tipo J
            inst->type_inst = 1;
            inst->addr = bits_jump(b);
            inst->rs = inst->rt = inst->rd = inst->funct = inst->imm = 0;
            
            s.reg_destino = 0; // destino = rt
            s.ula_fonte = 0; // operacao com registradores
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 1; // salta
            s.controle_ula = 0; //soma
            break;
        case 4: // Tipo I - Addi
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0; // destino = rt
            s.ula_fonte = 1; // operacao com = imm
            s.mem_para_reg = 0;
            s.esc_reg = 1; // escreve no banco
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0; // soma
        break;
        case 8: // Tipo I - Beq
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0; // destino = rt
            s.ula_fonte = 0; // operacao com registradores
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 0;
            s.branch = 1; // desvio
            s.jump = 0;
            s.controle_ula = 1; // subtracao
        break;
        case 11: // Tipo I - Lw
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0; // destino = rt
            s.ula_fonte = 1; // operacao com = imm
            s.mem_para_reg = 1; // pega da memoria
            s.esc_reg = 1; // escreve no registrador
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0; // soma
        break;
        case 15: // Tipo I - Sw
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0; // destino = rt
            s.ula_fonte = 1; // operacao com = imm
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 1; // escreve na memoria
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0; // soma
        break;
        default:
            inst->type_inst = 3;
        break;
    }
    return s;
}