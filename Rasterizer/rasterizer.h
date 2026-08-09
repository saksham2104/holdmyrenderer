#pragma once

#include "tgaimage.h"
#include "geometry.h"

void line(
    int ax, int ay,
    int bx, int by,
    TGAImage& framebuffer,
    TGAColor color
);

double triangle_area(
    const Point& a,
    const Point& b,
    const Point& c
);

void triangle(
    const Point& a,
    const Point& b,
    const Point& c,
    TGAImage& zbuffer,
    TGAImage& framebuffer,
    TGAColor color
);