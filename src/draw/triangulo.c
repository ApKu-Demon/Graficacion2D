#include "draw.h"
#include "linea.h"
#include "../math/vectores.h"
#include "figuras.h"

// Prototipos
void acomodo_vec(Triangulo *triangulo);
void fill_flat_bottom(Vec2 p1, Vec2 p3, Vec2 c, uint32_t color);
void fill_flat_top(Vec2 p1, Vec2 c, Vec2 p2, uint32_t color);

void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	linea_brhm(x0, y0, x1, y1, color);
	linea_brhm(x1, y1, x2, y2, color);
	linea_brhm(x2, y2, x0, y0, color);
}

void fill_trian(Triangulo triangulo)
{
	Triangulo *trian = &triangulo;
	acomodo_vec(trian);

	if(triangulo.p2.unpack.y == triangulo.p3.unpack.y)
	{
		fill_flat_bottom(triangulo.p1, triangulo.p3, triangulo.p2, triangulo.color.hex);
	}
	else if(triangulo.p1.unpack.y == triangulo.p2.unpack.y)
	{
		fill_flat_top(triangulo.p1, triangulo.p3, triangulo.p2, triangulo.color.hex);
	}
	else{
		int cy = triangulo.p2.unpack.y;
		int cx = (triangulo.p2.unpack.y - triangulo.p1.unpack.y) * (triangulo.p3.unpack.x - triangulo.p1.unpack.x) / (triangulo.p3.unpack.y - triangulo.p1.unpack.y) + triangulo.p1.unpack.x;

		Vec2 aux;
		aux.unpack.x = cx;
		aux.unpack.y = cy; 

		fill_flat_bottom(triangulo.p1, aux, triangulo.p2, triangulo.color.hex);
		fill_flat_top(aux, triangulo.p2, triangulo.p3, triangulo.color.hex);
	}
}

// . . . . . . . . . .


void fill_flat_bottom(Vec2 p1, Vec2 p3, Vec2 c, uint32_t color)
{
	// Clacula las pendientes
    float mi = (p3.unpack.x - p1.unpack.x) / (p3.unpack.y - p1.unpack.y); // pendiente izquierda
    float mf = (c.unpack.x - p1.unpack.x) / (c.unpack.y - p1.unpack.y);   // pendiente derecha

    // Inicializa las posiciones x para cada lado
    float xi = p1.unpack.x; // x izquierda
    float xf = p1.unpack.x; // x derecha

    // // linea de p1.y hasta c.y (de arriba hacia abajo)
    for (int y = (int)p1.unpack.y; y <= (int)c.unpack.y; y++) {
        linea_brhm(xi, y, xf, y, color); // dibuja una línea horizontal entre xi y xf
        xi += mi; // avanza x izquierda según la pendiente
        xf += mf; // avanza x derecha según la pendientes
    }
}

void fill_flat_top(Vec2 p3, Vec2 c, Vec2 p2, uint32_t color) {
    // Calcula las pendientes
    float mi = (p2.unpack.x - p3.unpack.x) / (p2.unpack.y - p3.unpack.y); // pendiente izquierda
    float mf = (p2.unpack.x - c.unpack.x) / (p2.unpack.y - c.unpack.y);   // pendiente derecha

    // Inicializa las posiciones x para cada lado
    float xi = p2.unpack.x;
    float xf = p2.unpack.x;

    // linea de p2.y hasta p1.y (de arriba hacia abajo)
    for (int y = (int)p2.unpack.y; y >= (int)p3.unpack.y; y--) {
        draw_linea(xi, y, xf, y, color); // dibuja una linea horizontal entre xi y xf
        xi -= mi; // retrocede x izquierda segun la pendiente
        xf -= mf; // retrocede x derecha segun la pendiente
    }
}

void acomodo_vec(Triangulo *triangulo)
{
	Vec2 *vectores[3] = {&triangulo->p1, &triangulo->p2, &triangulo->p3};
	Vec2 aux;

	for(int i=0; i<3; i++)
	{
		for(int j=i+1; j<3; j++)
		{
			if(vectores[i]->unpack.y > vectores[j]->unpack.y)
			{
				aux = *(vectores[i]);
				*(vectores[i]) = *(vectores[j]);
				*(vectores[j]) = aux;
			}
		}
	}
}

