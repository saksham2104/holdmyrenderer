#include "rasterizer.h"

#include <algorithm>
#include <cmath>

void line(
    int ax, int ay,
    int bx, int by,
    TGAImage& framebuffer,
    TGAColor color
) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);

    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    float y = ay;

    for (int x = ax; x <= bx; x++) {

        if (steep)
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);

        y += (by - ay) / static_cast<float>(bx - ax);
    }
}


double triangle_area(
    const Point& a,
    const Point& b,
    const Point& c
) 
{
    return 0.5 * (
        (b.y - a.y) * (b.x + a.x) +
        (c.y - b.y) * (c.x + b.x) +
        (a.y - c.y) * (a.x + c.x)
    );
}


void triangle(
    const Point& a,
    const Point& b,
    const Point& c,
    TGAImage& zbuffer,
    TGAImage& framebuffer,
    TGAColor color
) {
    int mn_x = std::min({a.x, b.x, c.x});
    int mx_x = std::max({a.x, b.x, c.x});

    int mn_y = std::min({a.y, b.y, c.y});
    int mx_y = std::max({a.y, b.y, c.y});

    double area = triangle_area(a, b, c);

    for (int x = mn_x; x <= mx_x; x++) {

        for (int y = mn_y; y <= mx_y; y++) {

            Point p(x, y);

            double alpha = triangle_area(p, b, c) / area;
            double beta  = triangle_area(p, c, a) / area;
            double gamma = triangle_area(p, a, b) / area;

            if (alpha < 0 || beta < 0 || gamma < 0)
                continue;

            unsigned char z =
                static_cast<unsigned char>(
                    alpha * a.z +
                    beta  * b.z +
                    gamma * c.z
                );

            if (z < zbuffer.get(x, y)[0])
                continue;

            framebuffer.set(x, y, color);

            zbuffer.set(x, y, {z});
        }
    }
}