//-----------------------------------------------------------------------------
// minivm.h -- Prototypes and definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef MINIVM_H
#define MINIVM_H


//---------------------------------------------------------
// MACRO DEFINITIONS:


// Size of the global function pointer table
#define MVM_NUM_FUNS 256

#define MVM_NUM_REGISTERS 16 // Default

#define MVM_MAX_MEM_SIZE  8192 // Default


//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

struct VMContext;

typedef void (*FunPtr)(struct VMContext* ctx, const uint32_t);

//CHANGE THE INTERNALS OF THIS FOR YOUR OWN VM!
typedef struct Reg {
    uint32_t type;
    uint32_t value;
} Reg;

typedef struct VMContext {
    uint32_t numRegs;
    uint32_t numFuns;
    Reg* r;           // Ptr to register array.
    FunPtr* funtable; // Ptr to a funptr table.
    char* code;       // Ptr to read instruction.
    char* mem;        // Ptr to memory space for execution/calculation.
} VMContext;


//---------------------------------------------------------
// ESOTERIC ITEMS:


#ifdef MVM_GLOBAL_FUNTABLE
// The global function pointer table.
static FunPtr mvm_function_table[MVM_NUM_FUNS];
#endif

// Byte extraction macros.
#define EXTRACT_B0(i) (i & 0xFF)
#define EXTRACT_B1(i) ((i >> 8) & 0xFF)
#define EXTRACT_B2(i) ((i >> 16) & 0xFF)
#define EXTRACT_B3(i) ((i >> 24) & 0xFF)

// Global variable that indicates if the process is running.
extern bool is_running;


//---------------------------------------------------------
// FUNCTIONS:


// Selects and executes an opcode function from the function pointer table.
// Passes the entire bytecode instruction as the argument.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr);

// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable);

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc);

// Stop the execution and exit
// halt :: VMContext -> uint32_t -> Effect()
void halt(struct VMContext* ctx, uint32_t instr);


//---------------------------------------------------------
#endif //ifndef MINIVM_H
