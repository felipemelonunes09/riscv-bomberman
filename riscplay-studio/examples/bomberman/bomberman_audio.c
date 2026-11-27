#include "bomberman_audio.h"

/* Frequências em Hz (0 = pausa/silêncio) */
#define R    0

#define F2   87
#define G2   98
#define A2   110
#define C3   131
#define D3   147
#define E3   165
#define G3   196
#define A3   220
#define B3   247

#define C4   262
#define D4   294
#define E4   330
#define F4   349
#define G4   392
#define A4   440
#define B4   494

#define C5   523
#define D5   587
#define E5   659
#define F5   698
#define G5   784
#define A5   880

#define ARRAY_LEN(arr) ((uint16_t)(sizeof(arr) / sizeof((arr)[0])))

/*
 * rp_note_t = {{RP_PULSE1 (melodia), RP_PULSE2 (harmonia), RP_TRIANGLE (baixo)}, noise, units}
 * units = duração em semicolcheias (16 units = 1 compasso 4/4)
 */
static const rp_note_t pattern_a_notes[] = {
    /* Compasso 1: C */
    {{E5, C4, C3}, 0, 2},
    {{G5, E4, R }, 0, 2},
    {{C5, G3, G2}, 1, 2},
    {{E5, C4, R }, 0, 2},
    {{G5, E4, C3}, 0, 2},
    {{E5, C4, R }, 0, 2},
    {{D5, B3, G2}, 1, 2},
    {{C5, G3, R }, 0, 2},

    /* Compasso 2: F -> G */
    {{F5, A3, F2}, 0, 2},
    {{A5, C4, R }, 0, 2},
    {{G5, B3, G2}, 1, 2},
    {{F5, A3, R }, 0, 2},
    {{E5, G3, C3}, 0, 2},
    {{D5, B3, R }, 0, 2},
    {{G5, D4, G2}, 1, 3},
    {{R,  R,  R }, 0, 1},

    /* Compasso 3: C -> Am */
    {{E5, C4, C3}, 0, 2},
    {{G5, E4, R }, 0, 2},
    {{A5, E4, A2}, 1, 2},
    {{G5, C4, R }, 0, 2},
    {{E5, C4, A2}, 0, 2},
    {{C5, A3, R }, 0, 2},
    {{D5, A3, D3}, 1, 2},
    {{E5, C4, R }, 0, 2},

    /* Compasso 4: Dm -> G -> C */
    {{F5, A3, D3}, 0, 2},
    {{D5, B3, G2}, 0, 2},
    {{E5, C4, C3}, 1, 2},
    {{C5, G3, R }, 0, 2},
    {{D5, B3, G2}, 0, 2},
    {{B4, G3, R }, 0, 2},
    {{C5, E3, C3}, 1, 3},
    {{R,  R,  R }, 0, 1}
};

static const rp_note_t pattern_b_notes[] = {
    /* Compasso 1: F -> C */
    {{A5, F4, F2}, 0, 2},
    {{G5, C4, R }, 0, 2},
    {{F5, A3, F2}, 1, 2},
    {{A5, C4, R }, 0, 2},
    {{G5, E4, C3}, 0, 2},
    {{E5, C4, R }, 0, 2},
    {{C5, G3, G2}, 1, 4},

    /* Compasso 2: Dm -> G7 (volta para o Padrão A) */
    {{F5, D4, D3}, 0, 2},
    {{E5, A3, R }, 0, 2},
    {{D5, F4, D3}, 1, 2},
    {{F5, A3, R }, 0, 2},
    {{G5, D4, G2}, 0, 2},
    {{F5, B3, R }, 0, 2},
    {{D5, B3, G2}, 1, 2},
    {{B4, G3, D3}, 0, 2}
};

static const rp_pattern_t bomberman_patterns[] = {
    {pattern_a_notes, ARRAY_LEN(pattern_a_notes)},
    {pattern_b_notes, ARRAY_LEN(pattern_b_notes)}
};

/* Ordem da música: A -> A -> B em loop */
static const uint8_t bomberman_order[] = {0, 0, 1};

static const rp_song_t bomberman_song = {
    bomberman_patterns,
    ARRAY_LEN(bomberman_patterns),
    bomberman_order,
    ARRAY_LEN(bomberman_order),
    148, /* BPM */
    1    /* loop = 1 */
};

void bomberman_audio_init(void) {
    riscplay_audio_init();
    riscplay_audio_all_off();

    riscplay_audio_set_volume(RP_PULSE1, 8);
    riscplay_audio_set_volume(RP_PULSE2, 5);
    riscplay_audio_set_volume(RP_TRIANGLE, 7);
    riscplay_audio_set_volume(RP_NOISE, 3);

    riscplay_audio_play_song(&bomberman_song);
}

void bomberman_audio_tick(void) {
    /* elapsed_ms = 16 ms por quadro (~60 FPS) */
    riscplay_audio_update_ms(16);
}

void bomberman_audio_tick_ms(uint32_t elapsed_ms) {
    riscplay_audio_update_ms(elapsed_ms);
}

void bomberman_audio_walk(void) {
    riscplay_audio_sfx_tone(110, 35, 3);
}

void bomberman_audio_place_bomb(void) {
    riscplay_audio_sfx_tone(220, 70, 7);
}

void bomberman_audio_explode(void) {
    riscplay_audio_sfx_noise(1, 260, 10);
}

void bomberman_audio_powerup(void) {
    riscplay_audio_sfx_tone(880, 120, 8);
}