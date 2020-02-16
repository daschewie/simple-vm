#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "vm.h"


// ops is used for logging, assembling, and disassembling
INSTRUCTION ops[] = {
        {OP_NOOP, "noop", 0}, // No Operation - Do nothing

        // Stack
        {OP_LDIM, "ldim", 1}, // Load Immediate - Push immediate value to stack
        {OP_LDFP, "ldfp", 1}, // Push Frame relative value to stack
        {OP_LDHP, "ldhp", 1}, // Push Heap value to stack
        {OP_STFP, "stfp", 1}, // Pop stack value, and store in Frame relative stack location
        {OP_STHP, "sthp", 1}, // Pop stack value, and store in heap location
        {OP_DROP, "drop", 1}, // Pop stack value, and drop it.

//        {OP_DUP, "dup", 0}, // Pop stack value, and drop it.
//        {OP_OVER, "over", 0}, // Pop stack value, and drop it.
//        {OP_SWAP, "swap", 0}, // Pop stack value, and drop it.


        {OP_VARS, "vars", 1}, // Allocate space on stack for local variables

        // Branching
        {OP_LT, "lt", 0},     // If value at stack-1 is less than stack-0, push true on stack
        {OP_EQ, "eq", 0},     // If value at stack-1 equals stack-0, push true on stack
        {OP_BR,   "br",   1},     // Unconditional branch
        {OP_BRT,  "brt",  1},   // Pop value from stack, if value is true (1) then branch
        {OP_BRF,  "brf",  1},   // Pop value from stack, if value is false (0) then branch

        // Math
        {OP_ADD,  "add",  0},
        {OP_SUB,  "sub",  0},
        {OP_MUL,  "mul",  0},

        // Procedures
        {OP_TRAP, "trap", 1},
        {OP_CALL, "call", 2},
        {OP_RET,  "ret",  0},
        {OP_HALT, "halt", 0},

};


uint16_t heap[HEAP_SIZE];     // heap (global memory)
uint16_t stack[STACK_SIZE];   // general stack (local memory)
uint16_t r_stack[STACK_SIZE]; // return (linkage) stack


uint16_t ip = 0; // instruction pointer
int16_t fp = -1; // frame pointer
int16_t rp = -1; // return stack pointer
int16_t sp = -1; // general stack pointer

void trace_op(uint16_t *code) {
    int opcode = code[ip];
    printf ("%04X : %s", ip, ops[opcode].mnemonic);
    for (int arg=0; arg < ops[opcode].args; arg++) {
        printf (" %d", code[ip + arg + 1]);
    }
    printf ("\n");
}

void trace_stack() {

    printf("[ ");
    for (int i=0; i <= sp; i++) {
        printf("%d ", stack[i]);
    }
    printf("]\n");
}

int vm_exec(int16_t *code, uint16_t size, bool trace) {
    int16_t opcode = code[ip];
    int a,b,addr,offset;
    while (opcode != OP_HALT &&  ip < size) {
        if(trace) trace_op(code);
        ip++;
        switch (opcode) {
            case OP_NOOP:
                break;
            case OP_ADD:
                b = stack[sp--];   			// 2nd opnd at top of stack
                a = stack[sp--]; 			// 1st opnd 1 below top
                stack[++sp] = a + b;      	// push result
                break;
            case OP_SUB:
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a - b;
                break;
            case OP_MUL:
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a * b;
                break;
            case OP_LT :
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = (a < b) ? TRUE : FALSE;
                break;
            case OP_EQ :
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = (a == b) ? TRUE : FALSE;
                break;
            case OP_BR :
                ip = code[ip++];
                break;
            case OP_BRT :
                addr = code[ip++];
                if ( stack[sp--]==TRUE ) ip = addr;
                break;
            case OP_BRF :
                addr = code[ip++];
                if ( stack[sp--]==FALSE ) ip = addr;
                break;
            case OP_LDIM:
                stack[++sp] = code[ip++]; // push operand
                break;
            case OP_LDFP : // load local or arg
                offset = code[ip++];
                stack[++sp] = stack[fp + offset + 1];
                break;
            case OP_LDHP :// load from global memory
                addr = code[ip++];
                stack[++sp] = heap[addr];
                break;
            case OP_STFP :
                offset = code[ip++];
                stack[fp + offset + 1] = stack[sp--];
                break;
            case OP_STHP :
                addr = code[ip++];
                heap[addr] = stack[sp--];
                break;
            case OP_TRAP :
                addr = code[ip++];
                switch(addr) {
                    case 1: // print int from stack
                        printf("%d\n",stack[sp--]);
                        break;
                    default:
                        printf("ERROR: Invalid bios call.");
                        exit(1);
                }
                break;
            case OP_DROP:
                offset = code[ip++];
                sp -= offset;
                break;
            case OP_VARS:
                offset = code[ip++];
                sp += offset;
                break;
            case OP_CALL:
                addr = code[ip++];
                int nargs = code[ip++];
                r_stack[++rp] = nargs;
                r_stack[++rp] = fp;
                r_stack[++rp] = ip;
                fp = sp;
                ip = addr;
                break;
            case OP_RET:
                a = stack[sp--];      // pop return value
                ip = r_stack[rp--];     // pop return address and jump to it
                fp = r_stack[rp--];
                nargs = r_stack[rp--];  // how many args to throw away
                sp -= nargs;          // pop args
                stack[++sp] = a;      // leave result on stack
                break;
            default:
                printf("ERROR: Invalid opcode: %d at ip: %d", opcode, ip);
                exit(1);
        }
        if(trace) trace_stack();
        opcode = code[ip];
    }

    return 0;
}
