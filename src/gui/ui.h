#pragma once

#include "button.h"
#include "../draw/figuras.h"
#include <SDL3/SDL.h>
//#include <stddef.h>

typedef struct {
    Button* botones;
    size_t cantidad;

    // Estado de creacion de figura
    int esperando_click;
    TypeFig tipo_figura;
    Color color_figura;
} UIContext;

extern UIContext ui;

void init_ui(void);
void update_ui(SDL_Event* e);
void render_ui(void);
void render_ui_texto(void);

void activar_creacion_figura_tipo(TypeFig tipo);


