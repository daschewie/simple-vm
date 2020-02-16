//
// Created by dasch on 2/14/2020.
//
#include <stdbool.h>
#include "vm.h"

int16_t code[CODE_SIZE] = {
        OP_LDIM, 3, // 0
        OP_LDIM, 4, // 2
        OP_CALL, 10, 2, // 4
        OP_TRAP, 1, // 6
        OP_HALT, // 7

        OP_LDFP, -1,
        OP_LDFP, -2,
        OP_MUL, // 8
        OP_LDIM, 3, // 9
        OP_ADD, // 11
        OP_RET // 12
};

int main() {

    vm_exec(code, CODE_SIZE, true);
    return 0;
}