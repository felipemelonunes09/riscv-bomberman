#include "engine.h"
#include "context.h"

Context* engine_createContext() {
    return context_create();
}


void engine_draw(Engine *engine) {

    Character *player = &engine->ctx->player;
    
}

void engine_flip(Engine *engine) {
    // Implement flip logic here using engine->ctx
}

void engine_init(Engine *engine) {
    engine->ctx = engine_createContext();
}