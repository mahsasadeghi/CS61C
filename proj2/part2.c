#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

#define FALSE 0
#define TRUE 1

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    // Variable Declaration:    Should I declare all the variables I use here or it is ok to declare them in
    //                          in the case that it is using it.
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                    // funct == 0x00 (Shift Left Logical)
                case 0x00:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt; // unsigned for shamt
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x02 (Shift Right Logical)      // Does it need to be unsigned?
                case 0x02:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x03 (Shift Right Arithmetic)
                case 0x03:
                    processor->R[instruction.rtype.rd] = ((sWord)processor->R[instruction.rtype.rt]) >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x08 (Jump Register)
                case 0x08:
                    processor->PC = processor->R[instruction.rtype.rs];
                    break;
                    
                    // funct == 0x09 (JALR)
                case 0x09:{
                    //Variable Declaration
                    Word tmp;
                    
                    tmp = processor->PC + 4;
                    processor->PC = processor->R[instruction.rtype.rs];
                    processor->R[instruction.rtype.rd] = tmp;
                    break;
                }
                    
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x10 (Move from HI)
                case 0x10:
                    processor->R[instruction.rtype.rd] = processor->RHI;
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x12 (Move from LO)
                case 0x12:
                    processor->R[instruction.rtype.rd] = processor->RLO;
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x18 (Multiply)
                case 0x18:{
                    // Variable Declaration
                    sDouble tmp, lTemp, hTemp;
                    
                    tmp = ((sDouble)processor->R[instruction.rtype.rs]) * ((sDouble)processor->R[instruction.rtype.rt]);
                    lTemp = tmp & (0x00000000ffffffff);     // lower 32 bits of tmp
                    hTemp = tmp & (0xffffffff00000000);     // upper 32 bits of tmp
                    
                    processor->RLO = (sWord)lTemp;
                    hTemp = hTemp >> 32;
                    processor->RHI = (sWord)hTemp;
                    processor->PC += 4;
                    break;
                }
                    
                    // funct == 0x19 (Multiply Unsigned)
                case 0x19:{
                    // Variable Declaration
                    Double tmp, lTemp, hTemp;
                    
                    tmp = ((Double)processor->R[instruction.rtype.rs]) * ((Double)processor->R[instruction.rtype.rt]);  // Do we need to cast it?
                    lTemp = tmp & (0x00000000ffffffff);     // lower 32 bits of tmp
                    hTemp = tmp & (0xffffffff00000000);     // upper 32 bits of tmp
                    
                    processor->RLO = (Word)lTemp;
                    hTemp = hTemp >> 32;
                    processor->RHI = (Word)hTemp;
                    processor->PC += 4;
                    break;
                }
                    
                    // funct == 0x21 (Add Unsigned)
                case 0x21:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x23 (Subtract Unsigned)
                case 0x23:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x25 (OR)
                case 0x25:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x26 (XOR)
                case 0x26:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x27 (NOR)
                case 0x27:
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x2a (Set on Less than (signed))
                case 0x2a:
                    processor->R[instruction.rtype.rd] = (signed)processor->R[instruction.rtype.rs] < (sWord)processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                    // funct == 0x2b (Set on Less than (unsigned))
                case 0x2b:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                    
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
            
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
            
            // opcode == 0x03 (Jump and Link)
        case 0x03:
            processor->R[31] = processor->PC + 4;
            processor->PC = ((processor->PC + 4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
            
            // opcode == 0x04 (Branch on Equal)
        case 0x04:
            if(processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt])
                processor->PC += 4 + ((sWord)(sHalf)instruction.itype.imm << 2);
            else
                processor->PC += 4;
            break;
            
            // opcode == 0x05 (Branch on Not Equal)
        case 0x05:
            if(processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt])
                processor->PC += 4 + ((sWord)(sHalf)instruction.itype.imm << 2);
            else
                processor->PC += 4;
            break;
            
            // opcode == 0x09 (Add immediate unsigned)
        case 0x09:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm;
            processor->PC += 4;
            break;
            
            // opcode == 0x0a (Set on Less than Immediate)
        case 0x0a:
            processor->R[instruction.itype.rt] = (signed)processor->R[instruction.itype.rs] < (sWord)(sHalf)instruction.itype.imm;
            processor->PC += 4;
            break;
            
            // opcode == 0x0b (Set on Less than Immediate Unsigned)
        case 0x0b:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] < (sWord)(sHalf)instruction.itype.imm;
            processor->PC += 4;
            break;
            
            // opcode == 0xc (ANDI)
        case 0x0c:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
            
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
            
            // opcode == 0xe (XORI)
        case 0x0e:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
            
            // opcode == 0xf (Load Upper Immediate)
        case 0x0f:
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;
            
            // opcode == 0x20 (Load Byte)
        case 0x20:
            processor->R[instruction.itype.rt] = (sByte)load(memory, processor->R[instruction.itype.rs] + (sHalf)instruction.itype.imm, LENGTH_BYTE);
            processor->PC +=4;
            break;
            
            // opcode == 0x21 (Load H)
        case 0x21:
            processor->R[instruction.itype.rt] = (sHalf)load(memory, processor->R[instruction.itype.rs] + (sHalf)instruction.itype.imm, LENGTH_HALF_WORD);
            processor->PC +=4;
            break;
            
            // opcode == 0x23 (Load Word)
        case 0x23:
            processor->R[instruction.itype.rt] = load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_WORD);
            processor->PC += 4;
            break;
            
            // opcode == 0x24 (Load Byte Unsigned)
        case 0x24:
            processor->R[instruction.itype.rt] = load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_BYTE);
            processor->PC += 4;
            break;
            
            // opcode == 0x25 (Load H Unsigned)
        case 0x25:
            processor->R[instruction.itype.rt] = load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
            
            // opcode == 0x28 (Store Byte)
        case 0x28:
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
            
            // opcode == 0x29 (Store H)
        case 0x29:
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
            
            // opcode == 0x2b (Store Word)
        case 0x2b:
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
            
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {
    
    /* YOUR CODE HERE */                                // should it be >=  ??
    if(address < 1 || (address >= MEMORY_SPACE) || (address % alignment != 0))
        return FALSE;
    
    return TRUE;
    //return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    switch(alignment){
        case LENGTH_BYTE:
            *(Byte*)(memory + address) = (Byte)(value);  // Is this because of little endiance?
            break;
            
        case LENGTH_HALF_WORD:
            *(Half*)(memory + address) = (Half)(value);
            break;
            
        case LENGTH_WORD:
            *(Word*)(memory + address) = (Word)value;
            break;
    }
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    switch(alignment){
        case LENGTH_BYTE:
            return (Word) *(Byte*)(memory + address);
        case LENGTH_HALF_WORD:
            return (Word) *(Half*)(memory + address);
        case LENGTH_WORD:
            return *(Word*)(memory + address);
    }
    
    
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    return *(Word*)(memory+address);
}
