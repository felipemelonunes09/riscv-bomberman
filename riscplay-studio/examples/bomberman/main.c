#include "engine.h"
#include "context.h"
#include "bomberman_audio.h"

static Engine engine;

int main(void) {

    engine_init(&engine);
    bomberman_audio_init();

    for (;;) {
        engine_flip(&engine);
        engine_draw(&engine);
        bomberman_audio_tick();
    }
}