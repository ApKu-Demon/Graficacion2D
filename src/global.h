#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>
#include "color/colores.h"
#include "draw/figuras.h"

typedef struct _estado
{
	SDL_Renderer *renderer;
	SDL_Texture *textura;
	SDL_Event evento;
	uint32_t *color_buffer;
	int ven_width;
	int ven_height;
	Color clear_color;
	int run;
	Figuras *figuras_buffer;
}EstadosRender;

extern EstadosRender estadosrender;
