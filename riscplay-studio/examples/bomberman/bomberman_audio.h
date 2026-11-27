#ifndef BOMBERMAN_AUDIO_H
#define BOMBERMAN_AUDIO_H

#include <stdint.h>
#include "riscplay_audio.h"

void bomberman_audio_init(void);
void bomberman_audio_tick(void);
void bomberman_audio_tick_ms(uint32_t elapsed_ms);

/* Efeitos sonoros (usam RP_PULSE2 e RP_NOISE sem cortar a melodia em RP_PULSE1) */
void bomberman_audio_walk(void);
void bomberman_audio_place_bomb(void);
void bomberman_audio_explode(void);
void bomberman_audio_powerup(void);

#endif