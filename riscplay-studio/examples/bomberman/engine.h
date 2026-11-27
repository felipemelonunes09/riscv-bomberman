#ifndef ENGINE_RISCPLAY_GAME_H
#define ENGINE_RISCPLAY_GAME_H
#include "context.h"

typedef struct {
    Context *ctx;
} Engine;

// ppu format: .insn r opcode, funct3, funct7, rd, rs1, rs2 (??) * - *
// insn     - it`s a directive to tell the assembler to genererate a machine instruction manually
// r        - it`s a type of instruction (r-type, i-type, s-type, b-type, u-type, j-type) 
// 0x2B     - it`s a reserved area to personalized instructions
// 0x0      - 
// 0x00     -
// %0       -
// %1       -
void process_pixelUnit(uint32_t rs1, uint32_t rs2)
{
    __asm__ volatile(
        ".insn r 0x2B, 0x0, 0x00, x0, %0, %1"
        :
        : "r"(rs1), "r"(rs2)
        : "memory"
    );
}

void engine_init(Engine *engine);
void engine_draw(Engine *engine);
void engine_flip(Engine *engine);
void engine_kill(Engine *engine);

Context* engine_createContext(void);

#endif