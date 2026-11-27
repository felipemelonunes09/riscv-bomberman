#ifndef BOMBERMAN_ASSETS_H
#define BOMBERMAN_ASSETS_H

#include <stdint.h>

#define CHAR_WIDTH 16u
#define CHAR_HEIGHT 24u

typedef enum {
    ASSET_CHAR_WALK_LEFT_0 = 0,
    ASSET_CHAR_WALK_LEFT_1,
    ASSET_CHAR_WALK_LEFT_2,
    ASSET_CHAR_WALK_RIGHT_0,
    ASSET_CHAR_WALK_RIGHT_1,
    ASSET_CHAR_WALK_RIGHT_2,
    ASSET_CHAR_WALK_DOWN_0,
    ASSET_CHAR_WALK_DOWN_1,
    ASSET_CHAR_WALK_DOWN_2,
    ASSET_CHAR_WALK_UP_0,
    ASSET_CHAR_WALK_UP_1,
    ASSET_CHAR_WALK_UP_2,
    ASSET_CHAR_DEAD_0,
    ASSET_CHAR_DEAD_1,
    ASSET_CHAR_DEAD_2,
    ASSET_CHAR_DEAD_3,
    ASSET_CHAR_DEAD_4,
    ASSET_CHAR_DEAD_5,
    ASSET_CHAR_DEAD_6,
    ASSET_COUNT,
    ASSET_PLAYER_IDLE = ASSET_CHAR_WALK_RIGHT_0
} AssetState;

typedef struct {
    uint32_t palette;
    uint32_t pixels;
    uint16_t width;
    uint16_t height;
} Asset;

extern const Asset assets[ASSET_COUNT];

#endif