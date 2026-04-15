#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
sinais decoder(instrucao *inst, CPU *cpu) {
    char *b = inst->inst_bin;
    sinais s;
    inst->opcode = separa_bits(b, 0, 4);

    switch (inst->opcode) {

        case 0: // Tipo R
            inst->type_inst = 0;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->rd = separa_bits(b, 10, 3);
            inst->funct = separa_bits(b, 13, 3);
            inst->imm = inst->addr = 0;

            s.reg_destino = 1;
            s.ula_fonte = 0;
            s.mem_para_reg = 0;
            s.esc_reg = 1;
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;

            switch(inst->funct){
                case 0: s.controle_ula = 0; break;
                case 1: s.controle_ula = 1; break;
                case 2: s.controle_ula = 2; break;
                case 3: s.controle_ula = 3; break;
                default: printf("Funct invalido\n"); break;
            }
        break;

        case 2: // JUMP
            inst->type_inst = 1;
            inst->addr = bits_jump(b);
            inst->rs = inst->rt = inst->rd = inst->funct = inst->imm = 0;

            s.reg_destino = 0;
            s.ula_fonte = 0;
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 1;
            s.controle_ula = 0;
            break;

    
        case 4: // ADDI
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0;
            s.ula_fonte = 1;
            s.mem_para_reg = 0;
            s.esc_reg = 1;
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0;
        break;

        case 8: // BEQ
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0;
            s.ula_fonte = 0;
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 0;
            s.branch = 1;
            s.jump = 0;
            s.controle_ula = 1;
        break;

        case 11: // LW
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0;
            s.ula_fonte = 1;
            s.mem_para_reg = 1;
            s.esc_reg = 1;
            s.esc_mem = 0;
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0;
        break;

        case 15: // SW
            inst->type_inst = 2;
            inst->rs = separa_bits(b, 4, 3);
            inst->rt = separa_bits(b, 7, 3);
            inst->imm = bits_imm(b, 10, 6);
            inst->rd = inst->funct = inst->addr = 0;

            s.reg_destino = 0;
            s.ula_fonte = 1;
            s.mem_para_reg = 0;
            s.esc_reg = 0;
            s.esc_mem = 1;
            s.branch = 0;
            s.jump = 0;
            s.controle_ula = 0;
        break;

        default:
            inst->type_inst = 3;
        break;
    }

    return s;
}