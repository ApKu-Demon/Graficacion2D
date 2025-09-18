#include "button.h"
#include "../draw/draw.h"
#include "../draw/figuras.h"
#include "../global.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

extern TTF_Font* font;



void draw_button(Button* btn) {
    //SDL_Log("Texto: %s", btn->texto);
    uint32_t color = btn->normal;
    if (btn->state == 1) color = btn->hover;
    else if (btn->state == 2) color = btn->pressed;

    // Fondo del boton
    draw_cuadrado(btn->cuadro.pos.unpack.x, btn->cuadro.pos.unpack.y,
                  btn->cuadro.w, btn->cuadro.h, color);

    float cx = btn->cuadro.pos.unpack.x + btn->cuadro.w / 2.0f;
    float cy = btn->cuadro.pos.unpack.y + btn->cuadro.h / 2.0f;

    //fill(btn->cuadro.pos, color);

    // Renderizar figura si aplica
    if (btn->tipo == BOTON_FIGURA || btn->tipo == BOTON_MIXTO) {
        Figuras* fig = &btn->figura_interna;

        switch (fig->data.type) {
            case CIRC:
                fig->circulo.pos.unpack.x = cx;
                fig->circulo.pos.unpack.y = cy;
                break;
            case CUADRO:
                fig->cuadro.pos.unpack.x = cx - fig->cuadro.w / 2.0f;
                fig->cuadro.pos.unpack.y = cy - fig->cuadro.h / 2.0f;
                break;
            case TRIAN:
                fig->triangulo.p1.unpack.x = cx;
                fig->triangulo.p1.unpack.y = cy - 20;

                fig->triangulo.p2.unpack.x = cx - 20;
                fig->triangulo.p2.unpack.y = cy + 20;

                fig->triangulo.p3.unpack.x = cx + 20;
                fig->triangulo.p3.unpack.y = cy + 20;
                break;
            default:
                break;
        }

        draw_figura(fig);
        fill_figura(fig);
    }
}

void draw_button_texto(Button* btn) {
    if ((btn->tipo == BOTON_TEXTO || btn->tipo == BOTON_MIXTO) && btn->texto) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, btn->texto, strlen(btn->texto), textColor);
        if (!surface) {
            SDL_Log("Error texto: %s", SDL_GetError());
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(estadosrender.renderer, surface);
        SDL_DestroySurface(surface);
        if (!texture) {
            SDL_Log("Error al crear textura de texto");
            return;
        }

        SDL_SetRenderDrawBlendMode(estadosrender.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        float textW, textH;
        SDL_GetTextureSize(texture, &textW, &textH);

        float cx = btn->cuadro.pos.unpack.x + btn->cuadro.w / 2.0f;
        float cy = btn->cuadro.pos.unpack.y + btn->cuadro.h / 2.0f;

        SDL_FRect dest = { cx - textW / 2.0f, cy - textH / 2.0f, textW, textH };
        SDL_RenderTexture(estadosrender.renderer, texture, NULL, &dest);

        SDL_DestroyTexture(texture);
    }
}

void update_button_state(Button* btn, SDL_Event* e) {
    float mx = e->motion.x;
    float my = e->motion.y;

    int dentro = mx >= btn->cuadro.pos.unpack.x &&
                 mx <= btn->cuadro.pos.unpack.x + btn->cuadro.w &&
                 my >= btn->cuadro.pos.unpack.y &&
                 my <= btn->cuadro.pos.unpack.y + btn->cuadro.h;

    if (e->type == SDL_EVENT_MOUSE_MOTION) {
        btn->state = dentro ? 1 : 0;
    } else if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN && dentro) {
        btn->state = 2;
        if (btn->on_click) btn->on_click();
    } else if (e->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        btn->state = 0;
    }
}


