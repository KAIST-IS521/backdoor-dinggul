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
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->mem        = (char*)malloc(MVM_MAX_MEM_SIZE);
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = **pc;

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);

    // Increment to next instruction.
    (*pc)++;
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

