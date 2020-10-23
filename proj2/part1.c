#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"

void decode_instruction(Instruction instruction) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0 (SPECIAL)
            switch(instruction.rtype.funct) {
                    // funct == 0x00 (Shift Left Logical)
                case 0x00:
                    printf("sll\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                    // funct == 0x02 (Shift Right Logical)
                case 0x02:
                    printf("srl\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                    // funct == 0x03 (Shift Right Arithmetic)
                case 0x03:
                    printf("sra\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                    // funct == 0x08 (Jump Register)
                case 0x08:
                    printf("jr\t$%d\n",instruction.rtype.rs);
                    break;
                    // funct == 0x09 ()
                case 0x09:
                    printf("jalr\t$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs);
                    break;
                case 0xc: // funct == 0xc (SYSCALL)
                    printf("syscall\n");
                    break;
                    // funct == 0x10 (Move from HI)
                case 0x10:
                    printf("mfhi\t$%d\n",instruction.rtype.rd);
                    break;
                    // funct == 0x12 (Move from LO)
                case 0x12:
                    printf("mflo\t$%d\n",instruction.rtype.rd);
                    break;
                    // funct == 0x18 (Multiply)
                case 0x18:
                    printf("mult\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x19 (Multiply Unsigned)
                case 0x19:
                    printf("multu\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x21 (Add Unsigned)
                case 0x21:
                    printf("addu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x23 (Subtract Unsigned)
                case 0x23:
                    printf("subu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x24: // funct == 0x24 (AND)
                    printf("and\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x25 (OR)
                case 0x25:
                    printf("or\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x26 (XOR)
                case 0x26:
                    printf("xor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x27 (NOR)
                case 0x27:
                    printf("nor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x2a (Set on Less than (signed))
                case 0x2a:
                    printf("slt\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                    // funct == 0x2b (Set on Less than (unsigned))
                case 0x2b:
                    printf("sltu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: illegal function: %08x\n",__FUNCTION__,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x02 (J)
            printf("j\t0x%x\n",instruction.jtype.addr*4);
            break;
            // opcode == 0x03 (Jump and Link)
        case 0x3:
            printf("jal\t0x%x\n",instruction.jtype.addr*4);
            break;
            // opcode == 0x04 (Branch on Equal)
        case 0x04:
            printf("beq\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, (int16_t)instruction.itype.imm*4);
            break;
            // opcode == 0x05 (Branch on Not Equal)
        case 0x05:
            printf("bne\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, (int16_t)instruction.itype.imm*4);
            break;
            // opcode == 0x09 (Add immediate unsigned)
        case 0x09:
            printf("addiu\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, (int16_t)instruction.itype.imm);
            break;
            // opcode == 0x0a (Set on Less than Immediate)
        case 0x0a:
            printf("slti\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, (int16_t)instruction.itype.imm);
            break;
            // opcode == 0x0b (Set on Less than Immediate Unsigned)
        case 0x0b:
            printf("sltiu\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, (int16_t)instruction.itype.imm);
            break;
            // opcode == 0xc (ANDI)
        case 0x0c:
            printf("andi\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs, instruction.itype.imm);
            break;
        case 0xd: // opcode == 0xd (ORI)
            printf("ori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
            // opcode == 0xe (XORI)
        case 0x0e:
            printf("xori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs, instruction.itype.imm);
            break;
            // opcode == 0xf (Load Upper Immediate)
        case 0x0f:
            printf("lui\t$%d,0x%x\n",instruction.itype.rt,instruction.itype.imm);
            break;
            // opcode == 0x20 (Load Byte)
        case 0x20:
            printf("lb\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x21 (Load H)
        case 0x21:
            printf("lh\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x23 (Load Word)
        case 0x23:
            printf("lw\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x24 (Load Byte Unsigned)
        case 0x24:
            printf("lbu\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x25 (Load H Unsigned)
        case 0x25:
            printf("lhu\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x28 (Store Byte)
        case 0x28:
            printf("sb\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x29 (Store H)
        case 0x29:
            printf("sh\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
            // opcode == 0x2b (Store Word)
        case 0x2b:
            printf("sw\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: illegal instruction: %08x\n",__FUNCTION__,instruction.bits);
            exit(-1);
            break;
    }
}
