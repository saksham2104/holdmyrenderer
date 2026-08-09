//g++-16 -std=c++23 main.cpp model.cpp rasterizer.cpp tgaimage.cpp -o main && ./main && open main.tga

//  time ./main -> to compare time

#include <cstdlib>
#include <iostream>

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "rasterizer.h"

constexpr int width = 800;
constexpr int height = 800;

Point convert(const Vertice& v) {

    int x = (v.x + 1) * width / 2;
    int y = (v.y + 1) * height / 2;
    int z = (v.z + 1) * 255 / 2;

    return Point{x, y, z};
}

int main() {

    TGAImage framebuffer(
        width,
        height,
        TGAImage::RGB
    );

    TGAImage zbuffer(
        width,
        height,
        TGAImage::GRAYSCALE
    );

    Model model("african_head.obj");

    std::cout<< "Vertices: " << model.get_vertices().size()<< '\n';
        
    std::cout<< "Faces: " << model.get_faces().size()<< '\n';
    

    for (const Face& f : model.get_faces()) {

        Vertice v0 = model.get_vertices()[f.a];
        Vertice v1 = model.get_vertices()[f.b];
        Vertice v2 = model.get_vertices()[f.c];

        Point a = convert(v0);
        Point b = convert(v1);
        Point c = convert(v2);

        TGAColor color;

        for (int i = 0; i < 3; i++)
            color[i] = std::rand() % 255;

        triangle(a, b, c,zbuffer,framebuffer,color);
    }

    framebuffer.write_tga_file("main.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    return 0;
}