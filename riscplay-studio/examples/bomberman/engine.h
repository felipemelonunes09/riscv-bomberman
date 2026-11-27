#ifndef ENGINE_RISCPLAY_GAME_H
#define ENGINE_RISCPLAY_GAME_H
#include "context.h"

typedef struct {
    Context *ctx;
} Engine;

// ppu format: .insn r opcode, funct3, funct7, rd, rs1, rs2 (??) * - *
void process_pixelUnit()
{
    __asm__ volatile(".insn ")
}

void engine_init(Engine *engine);
void engine_draw(Engine *engine);
void engine_flip(Engine *engine);
void engine_kill(Engine *engine);

Context* engine_createContext(void);

#endif