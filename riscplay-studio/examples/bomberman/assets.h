#ifndef BOMBERMAN_ASSETS_H
#define BOMBERMAN_ASSETS_H
#define CHAR_WIDTH 16u
#define CHAR_HEIGHT 24u

#include <stdint.h>

typedef enum {
    ASSET_PLAYER_IDLE    =   0,    
    ASSET_COUNT
} AssetState;


typedef struct {
    uint32_t palette;
    uint32_t pixels;
    uint16_t width;
    uint16_t height;
} Asset;

extern const Asset assets[ASSET_COUNT];

#endif