#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "draw/linea.h"
#include "global.h"
#include "math/vectores.h"
#include "draw/figuras.h"
#include "memoria/memoria.h"
#include "gui/ui.h"

#include <stdlib.h>
#include <math.h>

Vec2 *punto_seleccionado = NULL;

Vec2 *pivote_mas_cerca(Vec2 mp, Figuras* figs, float umbral)
{	
	int a = 0;
	int b = array_size(figs) - 1;

	float low;
	float high;
	float centro;
	
	for(int i=0; i<array_size(figs); i++)
	{
		int r = (a+b)/2;
		low = distanciav2(mp, figs[a].cuadro.pos);
		high = distanciav2(mp, figs[b].cuadro.pos);
		centro = distanciav2(mp, figs[r].cuadro.pos);

		if(fabs(low) <= umbral)
			return &figs[a].cuadro.pos;
		else if(fabs(high) <= umbral)
			return &figs[b].cuadro.pos;
		else if(fabs(centro) <= umbral)
			return &figs[r].cuadro.pos;

		if(r < b)
			b = r;
		else if(r > a)
			a = r;
	}
	return NULL;
}

void render_input(void)
{
	if(estadosrender.evento.type == SDL_EVENT_QUIT)
	{
		estadosrender.run =0;
	}

	if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		// Buscar el pivote mas cerca al mouse
		// {estructura		{arreglo}}
		printf("(%.3f, %.3f)\n", estadosrender.evento.motion.x, estadosrender.evento.motion.y);
		punto_seleccionado = pivote_mas_cerca((Vec2){{estadosrender.evento.motion.x, estadosrender.evento.motion.y}},
							estadosrender.figuras_buffer,
							10.f
							);

		if(punto_seleccionado)
		{
			printf("Punto cercano (%.3f, %.3f)\n", punto_seleccionado->unpack.x, punto_seleccionado->unpack.y);
		}
		
		// Obtener la referencia de la figura
	}

	else if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		if(punto_seleccionado)
		{
			// Modificar la figura
			punto_seleccionado->unpack.x = estadosrender.evento.motion.x;
			punto_seleccionado->unpack.y = estadosrender.evento.motion.y;
			punto_seleccionado = NULL;
		}
	}

	update_ui(&estadosrender.evento);
}

void clear_color_buffer(void) 
{
	for(int y=0; y<estadosrender.ven_height; ++y) 
	{
		for(int x=0; x<estadosrender.ven_width; ++x) 
		{
			draw_pixel(x, y, estadosrender.clear_color.hex);
		}
	}
}



void copy_buffer_to_texture(void) 
{
	SDL_UpdateTexture(estadosrender.textura,
			NULL,
			estadosrender.color_buffer,
			(int)(estadosrender.ven_width*sizeof(int)));

	SDL_RenderTexture(estadosrender.renderer, estadosrender.textura, NULL, NULL);
}



void _init(void) 
{
	TTF_Init();
	//SDL_SetTextureBlendMode(estadosrender.textura, SDL_BLENDMODE_BLEND);
	font = TTF_OpenFont("src/assets/fonts/Arial Black.ttf", 24);
	if (!font) {
		SDL_Log("Error al cargar fuente: %s", SDL_GetError());
		exit(1);
	}
	init_ui();
}



void update(void) 
{
	//SDL_SetRenderDrawColor(estadosrender.renderer, 0, 0, 0, 255); // fondo negro
	//SDL_RenderClear(estadosrender.renderer);

	// ciclo draw
    for (int i = 0; i < array_size(estadosrender.figuras_buffer); i++) {
        draw_figura(&estadosrender.figuras_buffer[i]);
        fill_figura(&estadosrender.figuras_buffer[i]);
    }

    render_ui();
	copy_buffer_to_texture();
	render_ui_texto();


    // Renderiza texto adicional (como "Fuente OK")
	/*
    if (font) {
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, "Fuente OK", strlen("Fuente OK"), color);
        if (!surface) return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(estadosrender.renderer, surface);
        SDL_DestroySurface(surface);
        if (!texture) return;

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        SDL_FRect dest = { 100, 100, w, h };
        SDL_Log("Renderizando texto en (%.2f, %.2f)", dest.x, dest.y);
		SDL_RenderTexture(estadosrender.renderer, texture, NULL, &dest);
		SDL_Log("Texto renderizado");
        SDL_DestroyTexture(texture);
    }

	SDL_SetRenderDrawColor(estadosrender.renderer, 0, 0, 0, 255);
	SDL_RenderClear(estadosrender.renderer);

	SDL_Color color = {255, 255, 255, 255};
	SDL_Surface* surface = TTF_RenderText_Blended(font, "TEST", strlen("TEST"), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(estadosrender.renderer, surface);
	SDL_DestroySurface(surface);

	float w, h;
	SDL_GetTextureSize(texture, &w, &h);
	SDL_FRect dest = {100, 100, w, h};
	SDL_RenderTexture(estadosrender.renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(estadosrender.renderer);
	*/
}



void render_frame(void) 
{
	SDL_RenderPresent(estadosrender.renderer);
}


