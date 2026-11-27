#include "context.h"


static Context context;

Context* context_create(void) {

    context.player.x = 152;
    context.player.y = 78;

    context.player.asset = ASSET_PLAYER_IDLE;

    return &context;
}   