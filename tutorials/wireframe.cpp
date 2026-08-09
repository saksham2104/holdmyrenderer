// g++-16 -std=c++23 wireframe.cpp tgaimage.cpp -o main && ./main && open wireframe.tga && time ./main

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "tgaimage.h"

using namespace std;

constexpr TGAColor red = { 0,0, 255, 255};


struct Vec3 {
    float x, y, z; // coordinates bw 0 to 1
};

struct Face {
    int a, b, c; // indice of Vertices (1 - indexed)
};

std::vector<Vec3> vertices;
std::vector<Face> faces;

void read_obj_file() {

    std::ifstream in("diablo3_pose.obj");

    if (!in.is_open()) {
        std::cerr << "Cannot open file\n";
        return;
    }

    std::string line;

    while (std::getline(in, line)) {

        std::istringstream iss(line);

        if (line.substr(0,2) == "v ") {

            char type;
            Vec3 v;

            iss >> type >> v.x >> v.y >> v.z;

            vertices.push_back(v);
        }

        else if (line.substr(0,2) == "f ") {

            char type;
            iss >> type;

            std::string s1, s2, s3;
            iss >> s1 >> s2 >> s3;

            Face f;

            f.a = std::stoi(s1.substr(0, s1.find('/'))) - 1;
            f.b = std::stoi(s2.substr(0, s2.find('/'))) - 1;
            f.c = std::stoi(s3.substr(0, s3.find('/'))) - 1;

            faces.push_back(f);        
        }
    }
}

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }
}


int main(int argc, char** argv) {

    constexpr int width  = 800;
    constexpr int height = 800;

    TGAImage framebuffer(width, height, TGAImage::RGB);

    read_obj_file();

    std::cout << "Vertices: " << vertices.size() << '\n';
    std::cout << "Faces: " << faces.size() << '\n';

    for (const Face &f : faces) {

        Vec3 v0 = vertices[f.a];
        Vec3 v1 = vertices[f.b];
        Vec3 v2 = vertices[f.c];

        int x0 = (v0.x + 1.) * width  / 2.;
        int y0 = (v0.y + 1.) * height / 2.;

        int x1 = (v1.x + 1.) * width  / 2.;
        int y1 = (v1.y + 1.) * height / 2.;

        int x2 = (v2.x + 1.) * width  / 2.;
        int y2 = (v2.y + 1.) * height / 2.;

        line(x0, y0, x1, y1, framebuffer, red);
        line(x1, y1, x2, y2, framebuffer, red);
        line(x2, y2, x0, y0, framebuffer, red);
    }

    framebuffer.write_tga_file("wireframe.tga");   
    return 0;

}
