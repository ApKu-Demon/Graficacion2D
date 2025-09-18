#pragma once

#include "../draw/figuras.h"
#include <stdint.h>
#include <SDL3/SDL.h>



typedef enum {
    BOTON_TEXTO,
    BOTON_FIGURA,
    BOTON_MIXTO
} TypeButton;

typedef struct {
    Cuadro cuadro;
    const char* texto;
    Figuras figura_interna;
    TypeButton tipo;
    uint32_t normal;
    uint32_t hover;
    uint32_t pressed;
    int state;                  // 0=normal, 1=hover, 2=pressed
    void (*on_click)(void);     // Accion al presionar
} Button;

void draw_button(Button* btn);
void draw_button_texto(Button* btn);
void update_button_state(Button* btn, SDL_Event* e);


