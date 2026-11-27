#include "engine.h"
#include "context.h"

static const uint32_t FRAMEBUFFER = 0xA080u;
static const uint32_t DARK_BLUE = 0xFFFF0000u;

Context* engine_createContext() {
    return context_create();
}

void process_pixelUnit(uint32_t rs1, uint32_t rs2)
{
    __asm__ volatile(
        ".insn r 0x2B, 0x0, 0x00, x0, %0, %1" : : "r"(rs1), "r"(rs2) : "memory"
    );
}


void wait_pixelUnit(void) {
    process_pixelUnit(0x50000000u, 0);
    __asm__ volatile(".insn r 0x2B, 0x1, 0x00, x0, x0, x0" : : : "memory");
}

static void select_framebuffer(uint32_t framebuffer)
{
    uint32_t offset = 0;
    __asm__ volatile(".insn r 0x2B, 0x0, 0x01, x0, %0, %1": : "r"(framebuffer), "r"(offset) : "memory");
}


void engine_draw(Engine *engine) {

    Character *player = &engine->ctx->player;
    
}

void engine_flip(Engine *engine) {
    // Implement flip logic here using engine->ctx
}

void engine_init(Engine *engine) {
    engine->ctx = engine_createContext();
    uint32_t fill_command;
    fill_command = 0x30000000u | (FRAMEBUFFER << 7);
    
    process_pixelUnit(fill_command, DARK_BLUE);
    wait_pixelUnit();
    select_framebuffer(FRAMEBUFFER);
}