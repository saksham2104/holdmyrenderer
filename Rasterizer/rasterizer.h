#pragma once

#include "tgaimage.h"
#include "geometry.h"

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);

double triangle_area(int ax, int ay, int bx, int by, int cx, int cy);

void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage& zbuffer, TGAImage& framebuffer, TGAColor color);
