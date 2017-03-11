// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

// Global variable that indicates if the process is running.
static bool is_running = true;

void usageExit() {
    // TODO: show usage
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
    // f[0x00] = halt;
    // f[0x10] = load;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[256];
    FunPtr f[256];
    FILE* bytecode;
    uint32_t* pc;
    uint32_t numRegs = sizeof(r) / sizeof(Reg);
    uint32_t numFuns = sizeof(f) / sizeof(FunPtr);

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, numRegs);
    // Initialize interpretation functions.
    initFuncs(f, numFuns);
    // Initialize VM context.
    initVMContext(&vm, numRegs, numFuns, r, f);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
        stepVMContext(&vm, &pc);
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
