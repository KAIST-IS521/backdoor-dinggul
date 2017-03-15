//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"
#include <stdio.h>
#include <string.h>

#define DEBUG 0

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:
//
// TODO: Make segfault message more clearly
//       e.g. [store] segfault at ins:[20000000] pc:[0x602030]


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    (*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> uint8_t* -> uint32_t** -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable,
                            uint8_t* code,
                            uint32_t codeLen,
                          uint32_t** pc) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->mem        = (uint8_t*)malloc(MVM_MAX_MEM_SIZE);
    ctx->code       = code;
    ctx->codeLen    = codeLen;
    ctx->pc         = pc;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = **pc;

    // Increment to next instruction.
    (*pc)++;

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);

    if ((uint64_t)*ctx->pc > (uint64_t)&ctx->code[ctx->codeLen-1]) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }
}

// Checks memory boundary and returns memory value according to offset.
// getMemValue :: VMContext -> uint32_t -> uint8_t
uint8_t getMemValue(struct VMContext* ctx, uint32_t offset) {
    if (offset >= MVM_MAX_MEM_SIZE) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }

    return ctx->mem[offset];
}

// Checks memory boundary and returns memory value according to offset.
// setMemValue :: VMContext -> uint32_t -> char -> Effect()
void setMemValue(struct VMContext* ctx, uint32_t offset, char value) {
    if (offset >= MVM_MAX_MEM_SIZE) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }

    *(ctx->mem+offset) = value;
}

// Prints chars
// print :: const char* -> Effect()
void print(const char* s) {
    int len = strlen(s);
    int i;
    for(i = 0; i < len; i++)
        putchar(s[i]);
}

// Stops the execution and exit.
// halt :: VMContext -> uint32_t -> Effect()
void halt(struct VMContext* ctx UNUSED, uint32_t instr UNUSED) {
#if DEBUG
    printf("halt:\tinstruction[%08x]\n", instr);
#endif
    // Now there is no next execution of instruction.
    is_running = false;
}

// Loads a 1byte value from memory into register.
// load :: VMContext -> uint32_t -> Effect()
void load(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("load:\tinstruction[%08x]\n", instr);
#endif
    uint8_t refVal = getMemValue(ctx, ctx->r[EXTRACT_B2(instr)].value);
    ctx->r[EXTRACT_B1(instr)].value = refVal;
}

// Stores a 1byte value from register into memory.
// store :: VMContext -> uint32_t -> Effect()
void store(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("store:\tinstruction[%08x]\n", instr);
#endif
    setMemValue(ctx,
                ctx->r[EXTRACT_B1(instr)].value,
                EXTRACT_B0(ctx->r[EXTRACT_B2(instr)].value));
}

// Moves a 32bit value from register to register.
// move :: VMContext -> uint32_t -> Effect()
void move(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("move:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = ctx->r[EXTRACT_B2(instr)].value;
}

// Moves an 1byte value to a register.
// puti :: VMContext -> uint32_t -> Effect()
void puti(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("puti:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = EXTRACT_B2(instr);
}

// Adds two values from registers and store to a register.
// add :: VMContext -> uint32_t -> Effect()
void add(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("add:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = ctx->r[EXTRACT_B2(instr)].value +
                                      ctx->r[EXTRACT_B3(instr)].value;
}

// Subtracts two values from registers and store to a register.
// sub :: VMContext -> uint32_t -> Effect()
void sub(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("sub:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = ctx->r[EXTRACT_B2(instr)].value -
                                      ctx->r[EXTRACT_B3(instr)].value;
}

// Compare two registers and store the result to a register. (greater than)
// gt :: VMContext -> uint32_t -> Effect()
void gt(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("gt:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = (ctx->r[EXTRACT_B2(instr)].value > ctx->r[EXTRACT_B3(instr)].value) ? 1 : 0;
}

// Compare two registers and store the result to a register. (grather & equal)
// ge :: VMContext -> uint32_t -> Effect()
void ge(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("ge:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = (ctx->r[EXTRACT_B2(instr)].value >= ctx->r[EXTRACT_B3(instr)].value) ? 1 : 0;
}

// Compare two registers and store the result to a register. (equal)
// eq :: VMContext -> uint32_t -> Effect()
void eq(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("eq:\tinstruction[%08x]\n", instr);
#endif
    ctx->r[EXTRACT_B1(instr)].value = (ctx->r[EXTRACT_B2(instr)].value == ctx->r[EXTRACT_B3(instr)].value) ? 1 : 0;
}

// Checks the first value of register. If it is greather than 0, set pc as the value of second register. Ohterwise, set pc as the value of third register.
// ite :: VMContext -> uint32_t -> Effect()
void ite(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("ite:\tinstruction[%08x]\n", instr);
#endif
    if (ctx->r[EXTRACT_B1(instr)].value) {
        if (ctx->r[EXTRACT_B2(instr)].value*4 >= ctx->codeLen) {
            fprintf(stderr, "Segmentation fault\n");
            exit(1);
        }

        *ctx->pc = (uint32_t*)ctx->code + EXTRACT_B2(instr);
    }
    else {
        if (ctx->r[EXTRACT_B3(instr)].value*4 >= ctx->codeLen) {
            fprintf(stderr, "Segmentation fault\n");
            exit(1);
        }

        *ctx->pc = (uint32_t*)ctx->code + EXTRACT_B3(instr);
    }
}

// Jumps to specified address.
// jump :: VMContext -> uint32_t -> Effect()
void jump(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("jump:\tinstruction[%08x]\n", instr);
#endif
    if (EXTRACT_B1(instr)*4 >= ctx->codeLen) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }

    *ctx->pc = (uint32_t*)ctx->code + EXTRACT_B1(instr);
}

// Same as libc puts except new line char at the end.
// _puts :: VMContext -> uint32_t -> Effect()
void _puts(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("puts:\tinstruction[%08x]\n", instr);
#endif
    int len = strlen((char *)&ctx->mem[ctx->r[EXTRACT_B1(instr)].value]);
    if (&ctx->mem[ctx->r[EXTRACT_B1(instr)].value+len] > &ctx->mem[MVM_MAX_MEM_SIZE]) {
        ctx->mem[MVM_MAX_MEM_SIZE] = 0;
        print((const char*)&ctx->mem[ctx->r[EXTRACT_B1(instr)].value]);
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }

    print((const char*)&ctx->mem[ctx->r[EXTRACT_B1(instr)].value]);
}

// Same as libc gets.
// _gets :: VMContext -> uint32_t -> Effect()
void _gets(struct VMContext* ctx, uint32_t instr) {
#if DEBUG
    printf("gets:\tinstruction[%08x]\n", instr);
#endif
    scanf("%s", ((char*)&ctx->mem[ctx->r[EXTRACT_B1(instr)].value]));

    int len = strlen((char *)&ctx->mem[ctx->r[EXTRACT_B1(instr)].value]);
    if (&ctx->mem[ctx->r[EXTRACT_B1(instr)].value+len] > &ctx->mem[MVM_MAX_MEM_SIZE]) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }
}

