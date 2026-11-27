#include "assets.h"

const Asset assets[ASSET_COUNT] = {
    [ASSET_PLAYER_IDLE] = {
        .palette    = 0x100000u,
        .pixels     = 0x100100u,
        .width      = 16,
        .height     = 24
    }
};