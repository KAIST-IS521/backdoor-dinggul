//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"

#define DEBUG 1

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


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
                          uint32_t** pc) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->mem        = (uint8_t*)malloc(MVM_MAX_MEM_SIZE);
    ctx->code       = code;
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
}

// Checks memory boundary and returns memory value according to offset.
// getMemValue :: VMContext -> uint32_t -> uint32_t
uint32_t getMemValue(struct VMContext* ctx, uint32_t offset) {
    if (offset+4 > MVM_MAX_MEM_SIZE) {
        fprintf(stderr, "Segmentation fault\n");
        exit(1);
    }

    return *(uint32_t*)(ctx->mem+offset);
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

// Stops the execution and exit.
// halt :: VMContext -> uint32_t -> Effect()
void halt(struct VMContext* ctx, uint32_t instr) {
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
    uint32_t refVal = getMemValue(ctx, ctx->r[EXTRACT_B2(instr)].value);
    ctx->r[EXTRACT_B1(instr)].value = EXTRACT_B0(refVal);
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
    if (ctx->r[EXTRACT_B1(instr)].value)
        *ctx->pc = ctx->code + (4 * ctx->r[EXTRACT_B2(instr)].value);
    else
        *ctx->pc = ctx->code + (4 * ctx->r[EXTRACT_B3(instr)].value);
}

