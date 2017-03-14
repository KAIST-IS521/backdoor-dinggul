// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
bool is_running = true;

void usageExit(char** argv) {
    fprintf(stderr, "Usage: %s <compiled_program>\n", argv[0]);
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
    f[0x00] = halt;
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
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;
    uint32_t* pc;

    // There should be at least one argument.
    if (argc < 2) usageExit(argv);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    fseek(bytecode, 0, SEEK_END);
    int fileLen = ftell(bytecode);
    rewind(bytecode);

    char* code = (char*)malloc(fileLen/4*4+4);
    fread(code, fileLen, 1, bytecode);

    fclose(bytecode);

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Initialize VM program counter.
    pc = (uint32_t*)code;
    while (is_running) {
        // TODO: Check the boundary of program counter.
        stepVMContext(&vm, &pc);
    }

    // Zero indicates normal termination.
    return 0;
}
