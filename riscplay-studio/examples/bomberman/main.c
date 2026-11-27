#include "engine.h"
#include "context.h"

static Engine engine;

int main(void) {

    engine_init(&engine);

    for (;;) {
        engine_flip(&engine);
        engine_draw(&engine);
    }
}