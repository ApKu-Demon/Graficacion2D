#include "ui.h"
#include "../global.h"
#include "../memoria/memoria.h"
#include "../draw/figuras.h"
#include <stdlib.h>

void activar_circulo(void) ;
void activar_cuadro(void);
void activar_triangulo(void);

UIContext ui = {0};

// Accion: activar modo de creacion de figura
void activar_creacion_figura(TypeFig tipo) {
    ui.esperando_click = 1;
    ui.tipo_figura = tipo;
    ui.color_figura = (Color){0xff00ffff}; // puedes cambiar color por boton
}

// Inicializar botones
void init_ui(void) {
    ui.cantidad = 3;
    ui.botones = malloc(sizeof(Button) * ui.cantidad);

    ui.botones[0] = (Button){
        .cuadro = {
            .pos = {.unpack = {.x = 50, .y = 600}},
            .w = 200,
            .h = 100,
            .color = {0x333333ff},
            .type = CUADRO
        },
        .tipo = BOTON_FIGURA,
        .figura_interna = {
            .circulo = {
                .pos = {{0, 0}}, .r = 20.f, .vert = 16, .borde = 1,
                .color = {0xffffffff}, .type = CIRC
            }
        },
        .normal = 0x333333ff,
        .hover = 0x555555ff,
        .pressed = 0x999999ff,
        .state = 0,
        .on_click = activar_circulo
    };

    ui.botones[1] = (Button){
        .cuadro = {
            .pos = {.unpack = {.x = 400, .y = 600}}, // centrado en pantalla
            .w = 200,
            .h = 80,
            .color = {0x222222ff},
            .type = CUADRO
        },
        .texto = "Crear figura",
        .tipo = BOTON_MIXTO,
        .figura_interna = {
            .cuadro = {
                .pos = {{0, 0}}, .w = 40, .h = 40,
                .color = {0xffffffff}, .type = CUADRO
            }
        },
        .normal = 0x222222ff,
        .hover = 0x444444ff,
        .pressed = 0x888888ff,
        .state = 0,
        .on_click = activar_cuadro
    };

    ui.botones[2] = (Button){
        .cuadro = {
            .pos = {.unpack = {.x = 750, .y = 600}}, // centrado en pantalla
            .w = 200,
            .h = 80,
            .color = {0x222222ff},
            .type = CUADRO
        },
        .tipo = BOTON_FIGURA,
        .figura_interna = {
            .triangulo = {
                .p1 = {{0, 0}}, .p2 = {{0, 0}}, .p3 = {{0, 0}},
                .color = {0xffffffff}, .type = TRIAN
            }
        },
        .normal = 0x333333ff,
        .hover = 0x555555ff,
        .pressed = 0x999999ff,
        .state = 0,
        .on_click = activar_triangulo
    };
}

// Actualizar estado de botones y clic para figura
void update_ui(SDL_Event* e) {
    for (size_t i = 0; i < ui.cantidad; ++i) {
        update_button_state(&ui.botones[i], e);
    }

    if (ui.esperando_click && estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float mx = estadosrender.evento.motion.x;
		float my = estadosrender.evento.motion.y;

		// Verificar si el clic fue dentro de algun boton
		int clic_en_boton = 0;
		for (size_t i = 0; i < ui.cantidad; ++i) {
			Button* b = &ui.botones[i];
			if (mx >= b->cuadro.pos.unpack.x &&
				mx <= b->cuadro.pos.unpack.x + b->cuadro.w &&
				my >= b->cuadro.pos.unpack.y &&
				my <= b->cuadro.pos.unpack.y + b->cuadro.h) {
				clic_en_boton = 1;
				break;
			}
		}

		if (!clic_en_boton) {
			Figuras nueva;

			switch (ui.tipo_figura) {
				case CIRC: {
					Circulo c = {
						.pos = {{mx, my}},
						.r = 30.f,
						.vert = 32,
						.borde = 1,
						.color = ui.color_figura,
						.type = CIRC
					};
					nueva.circulo = c;
					break;
				}
				case CUADRO: {
					Cuadro q = {
						.pos = {{mx, my}},
						.w = 60,
						.h = 60,
						.offset_mem1 = {0},
						.color = ui.color_figura,
						.type = CUADRO
					};
					nueva.cuadro = q;
					break;
				}

                case TRIAN: {
                    Triangulo t = {
                        .p1 = {{mx, my - 30}},
                        .p2 = {{mx - 30, my + 30}},
                        .p3 = {{mx + 30, my + 30}},
                        .color = ui.color_figura,
                        .type = TRIAN
                    };
                    nueva.triangulo = t;
                    break;
                }
				default: return;
			}

			pushto_array(estadosrender.figuras_buffer, nueva);
			ui.esperando_click = 0; //  desactiva modo espera
		}
	}
}

// Dibujar todos los botones
void render_ui(void) {
    for (size_t i = 0; i < ui.cantidad; ++i) {
        draw_button(&ui.botones[i]);
    }
}

void render_ui_texto(void) {
    for (size_t i = 0; i < ui.cantidad; ++i) {
        draw_button_texto(&ui.botones[i]);
    }
}

void activar_circulo(void)  { activar_creacion_figura(CIRC); }
void activar_cuadro(void)   { activar_creacion_figura(CUADRO); }
void activar_triangulo(void){ activar_creacion_figura(TRIAN); }


