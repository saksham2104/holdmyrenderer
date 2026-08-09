//g++-16 -std=c++23 main.cpp model.cpp rasterizer.cpp tgaimage.cpp -o main && ./main && open main.tga

//  time ./main -> to compare time

#include <cstdlib>
#include <iostream>
#include <tuple>

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "rasterizer.h"

constexpr int width = 800;
constexpr int height = 800;

std::tuple<int, int, int> project(Vec3 v) {
    return {(v.x + 1) * width / 2, (v.y + 1) * height / 2, (v.z + 1) * 255 / 2};
}

int main() {

    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    Model model("african_head.obj");

    std::cout<< "Vertices: " << model.get_vertices().size()<< '\n';
        
    std::cout<< "Faces: " << model.get_faces().size()<< '\n';
    

    for (const Face& f : model.get_faces()) {

        Vec3 v0 = model.get_vertices()[f.a];
        Vec3 v1 = model.get_vertices()[f.b];
        Vec3 v2 = model.get_vertices()[f.c];

        auto [ax, ay, az] = project(v0);
        auto [bx, by, bz] = project(v1);
        auto [cx, cy, cz] = project(v2);

        TGAColor color;

        for (int i = 0; i < 3; i++){
            color[i] = std::rand() % 255;
        }

        triangle(ax, ay, az, bx, by, bz, cx, cy, cz, zbuffer, framebuffer, color);
    }

    framebuffer.write_tga_file("main.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    return 0;
}
