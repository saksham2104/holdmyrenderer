#include "rasterizer.h"

#include <algorithm>
#include <cmath>

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
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


double triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return 0.5 * (
        (by - ay) * (bx + ax) +
        (cy - by) * (cx + bx) +
        (ay - cy) * (ax + cx)
    );
}

void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage& zbuffer, TGAImage& framebuffer, TGAColor color) {
    int mn_x = std::min({ax, bx, cx});
    int mx_x = std::max({ax, bx, cx});

    int mn_y = std::min({ay, by, cy});
    int mx_y = std::max({ay, by, cy});

    double area = triangle_area(ax, ay, bx, by, cx, cy);

    for (int x = mn_x; x <= mx_x; x++) {

        for (int y = mn_y; y <= mx_y; y++) {

            double alpha = triangle_area(x, y, bx, by, cx, cy) / area;
            double beta  = triangle_area(x, y, cx, cy, ax, ay) / area;
            double gamma = triangle_area(x, y, ax, ay, bx, by) / area;

            if (alpha < 0 || beta < 0 || gamma < 0)
                continue;

            unsigned char z =
                static_cast<unsigned char>(
                    alpha * az +
                    beta  * bz +
                    gamma * cz
                );

            if (z < zbuffer.get(x, y)[0])
                continue;

            framebuffer.set(x, y, color);

            zbuffer.set(x, y, {z});
        }
    }
}
