/*
  
TRIANLGE RASTERISATION


*/

//// g++-16 -std=c++23 triangle.cpp tgaimage.cpp -o main && ./main && open triangle.tga && time ./main


#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include "tgaimage.h"

using namespace std;

constexpr int width  = 100;
constexpr int height = 100;


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};


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
    float y = ay;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        y += (by-ay) / static_cast<float>(bx-ax);
    }
}

bool check(int px, int py, int ax, int ay, int bx, int by, int cx, int cy) {
    // create three lines 
    /*
     create line AB and then check sign of point C and point P && BC && CA
    */
    float m = (by - ay)/static_cast<float>(bx-ax);
    float d = ay - (m * ax);

    // line is y - mx - d

    return ((py - m * px -d) * (cy-m*cx -d) > 0);
    
}


void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {

    line(ax, ay, bx, by, framebuffer, color);
    line(bx, by, cx, cy, framebuffer, color);
    line(cx, cy, ax, ay, framebuffer, color);

    // fill triangle

    int mn_x = min({ax, bx, cx});
    int mx_x = max({ax, bx, cx});

    int mn_y = min({ay, by, cy});
    int mx_y = max({ay, by, cy});

    for(int x = mn_x;x<=mx_x;x++){
        for(int y =mn_y;y<=mx_y;y++){
            if(check(x, y, ax, ay, bx, by, cx, cy) && check(x, y, bx, by, cx, cy, ax, ay) && check(x, y, ax, ay, cx, cy, bx, by)){
                framebuffer.set(x, y, color);
            }
        }
    }
}




int main(int argc, char** argv) {


    TGAImage framebuffer(width, height, TGAImage::RGB);

    // draw a red triangle between A(1,1) , B(4,5) , C(3,4)

    const int ax = 12, ay = 11;
    const int bx = 74, by = 45;
    const int cx = 50,cy =  44;
    const int dx = 60 , dy = 30;

    triangle(bx, by, ax, ay, cx, cy, framebuffer, red);

    triangle(dx, dy, ax, ay, bx, by, framebuffer, white);

    framebuffer.write_tga_file("triangle.tga");
    return 0;
}
