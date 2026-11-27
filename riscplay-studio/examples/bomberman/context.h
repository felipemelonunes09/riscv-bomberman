#ifndef CONTEXT_RISCPLAY_GAME_H
#define CONTEXT_RISCPLAY_GAME_H

#include "assets.h"

typedef struct {
    int x;
    int y;
    AssetState asset;                         
} Character;

typedef struct {
} Map;

typedef struct {
    Map map;
    Character player;
    Character enemies[3];
} Context;


Context* context_create(void);

#endif
