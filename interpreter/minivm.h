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
    uint8_t* code;    // Ptr to a code space.
    uint8_t* mem;     // Ptr to memory space for execution/calculation.
    uint32_t** pc;    // Ptr to a program counter.
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
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> uint8_t* -> uint32_t** -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable,
                            uint8_t* code,
                          uint32_t** pc);

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc);

// Checks memory boundary and returns memory value according to offset.
// getMemValue :: VMContext -> uint32_t -> uint32_t
uint32_t getMemValue(struct VMContext* ctx, uint32_t offset);

// Checks memory boundary and returns memory value according to offset.
// setMemValue :: VMContext -> uint32_t -> char -> Effect()
void setMemValue(struct VMContext* ctx, uint32_t offset, char value);

// Stops the execution and exit.
// halt :: VMContext -> uint32_t -> Effect()
void halt(struct VMContext* ctx, uint32_t instr);

// Loads a 1byte value from memory into register.
// load :: VMContext -> uint32_t -> Effect()
void load(struct VMContext* ctx, uint32_t instr);

// Stores a 1byte value from register into memory.
// store :: VMContext -> uint32_t -> Effect()
void store(struct VMContext* ctx, uint32_t instr);

// Moves a 32bit value from register to register.
// move :: VMContext -> uint32_t -> Effect()
void move(struct VMContext* ctx, uint32_t instr);

// Moves an 1byte value to a register.
// puti :: VMContext -> uint32_t -> Effect()
void puti(struct VMContext* ctx, uint32_t instr);

// Adds two values from registers and store to a register.
// add :: VMContext -> uint32_t -> Effect()
void add(struct VMContext* ctx, uint32_t instr);

// Subtracts two values from registers and store to a register.
// sub :: VMContext -> uint32_t -> Effect()
void sub(struct VMContext* ctx, uint32_t instr);

// Compare two registers and store the result to a register. (greater than)
// gt :: VMContext -> uint32_t -> Effect()
void gt(struct VMContext* ctx, uint32_t instr);

// Compare two registers and store the result to a register. (grather & equal)
// ge :: VMContext -> uint32_t -> Effect()
void ge(struct VMContext* ctx, uint32_t instr);

// Compare two registers and store the result to a register. (equal)
// eq :: VMContext -> uint32_t -> Effect()
void eq(struct VMContext* ctx, uint32_t instr);


//---------------------------------------------------------
#endif //ifndef MINIVM_H
