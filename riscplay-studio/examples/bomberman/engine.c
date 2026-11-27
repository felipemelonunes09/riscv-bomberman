#include "engine.h"
#include "context.h"

static const uint32_t FRAMEBUFFER = 0xA080u;
static const uint32_t DARK_BLUE = 0xFF080C18u;

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

static void engine_drawAsset(const Asset* asset, int x, int y)
{
    uint32_t pixel_count;
    uint32_t destination;

    uint32_t palette_command;
    uint32_t copy_command;
    uint32_t render_command;
    uint32_t copy_parameters;
    uint32_t render_parameters;

    if (asset == 0) return ;
    palette_command = 0x10000000u | (asset->palette << 7);
    process_pixelUnit(palette_command, 0);

    pixel_count = (uint32_t)asset->width * asset->height;
    copy_command = asset->pixels << 7;
    copy_parameters = pixel_count << 2;

    process_pixelUnit(copy_command, copy_parameters);

    destination = FRAMEBUFFER + (uint32_t)(y * 320) + (uint32_t)x;

    render_command = 0x20000000u | (destination << 7) | (2u << 3) | ((asset->width & 0x70u) >> 4);
    render_parameters = ((uint32_t)(asset->width & 0x0Fu) << 28) | ((uint32_t)asset->height << 21) | (((uint32_t)x & 0x3FFu) << 11) | (((uint32_t)y & 0x1FFu) << 2);

    process_pixelUnit(render_command, render_parameters);
}


void engine_draw(Engine *engine) {

    Character *player = &engine->ctx->player;
    const Asset *player_asset;

    if ((unsigned)player->asset >= ASSET_COUNT) return;
    player_asset = &assets[player->asset];
    engine_drawAsset(player_asset, player->x, player->y);   

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