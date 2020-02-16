//
// Created by dasch on 2/14/2020.
//

#ifndef UNTITLED_VM_H
#define UNTITLED_VM_H

#include <stdint.h>

#define CODE_SIZE 1024
#define STACK_SIZE 1024
#define HEAP_SIZE 1024
#define TRUE 1
#define FALSE 0

#define OP_NOOP 0
#define OP_LDIM 1
#define OP_LDFP 2
#define OP_LDHP 3
#define OP_STFP 4
#define OP_STHP 5
#define OP_DROP 6
#define OP_VARS 7
#define OP_LT 8
#define OP_EQ 9
#define OP_BR 10
#define OP_BRT 11
#define OP_BRF 12
#define OP_ADD 13
#define OP_SUB 14
#define OP_MUL 15
#define OP_TRAP 16
#define OP_CALL 17
#define OP_RET 18
#define OP_HALT 19


typedef struct INSTRUCTION_t {
    uint8_t opcode;
    char *mnemonic;
    uint8_t args;
} INSTRUCTION;

int vm_exec(int16_t *code, uint16_t size, bool trace);

#endif //UNTITLED_VM_H
