#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "draw/linea.h"
#include "global.h"
#include "math/vectores.h"
#include "draw/figuras.h"
#include "memoria/memoria.h"

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
	//inicializar todas las variables que se usaran
	Vec2 pc1 = {{400.f, 360.f}};
	Vec2 pc2 = {{600.f, 100.f}};
	Vec2 pc3 = {{800.f, 360.f}};
	
	Linea linea = {
		.p1 = (Vec2){{800, 100}},
		.p2 = (Vec2){{600, 250}},
		.offset_mem1 = {0},
		.color = (Color){0xab00abff},
		.type = LINEA
	};

	Curva curva = {
		.p1 = pc1,
		.p2 = pc2,
		.p3 = pc3,
		.color = (Color){0xffff00ff},
		.type = CURVA
	};

	Circulo circulo = {
		.pos = {{estadosrender.ven_width/2.f, estadosrender.ven_height/2.f}},
		.r = 20.f,
		.vert = 32,
		.offset_mem1 = {0},
		.color = (Color){0x00ffffff},
		.type = CIRC
	};

	Cuadro cuadro = {
		.pos = {.unpack = {.x = 140, .y = 140}},
		.h = 200,
		.w = 200,
		.offset_mem1 = {0},
		.color = (Color){0x140140ff},
		.type = CUADRO
	};

	Triangulo triangulo = {
		.p1 = {.unpack = {.x = 400, .y = 620}},
		.p2 = {.unpack = {.x = 600, .y = 360}},
		.p3 = {.unpack = {.x = 800, .y = 620}},
		.color = (Color){0xffffffff},
		.type = TRIAN
	};

	Figuras test = {*(FigComun*)&linea};
	Figuras test2 = {.curva=curva};
	Figuras test3 = {*(FigComun*)&circulo};
	Figuras test4 = {.cuadro = cuadro};
	Figuras test5 = {.triangulo = triangulo};

	pushto_array(estadosrender.figuras_buffer, test);
	pushto_array(estadosrender.figuras_buffer, test2);
	pushto_array(estadosrender.figuras_buffer, test3);
	pushto_array(estadosrender.figuras_buffer, test4);
	pushto_array(estadosrender.figuras_buffer, test5);

}



void update(void) 
{
	/*
	Vec2 pc1 = {{400.f, 360.f}};
	Vec2 pc2 = {{600.f, 100.f}};
	Vec2 pc3 = {{800.f, 360.f}};
	
	Linea linea = {
		.p1 = (Vec2){{800, 100}},
		.p2 = (Vec2){{600, 250}},
		.offset_mem1 = {0},
		.color = (Color){0xab00abff},
		.type = LINEA
	};

	Curva curva = {
		.p1 = pc1,
		.p2 = pc2,
		.p3 = pc3,
		.color = (Color){0xffff00ff},
		.type = CURVA
	};

	Circulo circulo = {
		.pos = {{estadosrender.ven_width/2.f, estadosrender.ven_height/2.f}},
		.r = 20.f,
		.vert = 32,
		.offset_mem1 = {0},
		.color = (Color){0x00ffffff},
		.type = CIRC
	};

	Figuras test = {*(FigComun*)&linea};
	Figuras test2 = {.curva=curva};
	Figuras test3 = {*(FigComun*)&circulo};

	draw_figura(&test);
	draw_figura(&test2);
	draw_figura(&test3);
	*/

	// ciclo draw
	for(int i=0; i<array_size(estadosrender.figuras_buffer); i++)
	{
		draw_figura(&estadosrender.figuras_buffer[i]);
		fill_figura(&estadosrender.figuras_buffer[i]);
	}
}



void render_frame(void) 
{
	SDL_RenderPresent(estadosrender.renderer);
}


