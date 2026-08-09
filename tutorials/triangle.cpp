/*
  
TRIANLGE RASTERISATION


*/

//// g++-16 -std=c++23 triangle.cpp tgaimage.cpp -o main && ./main && open triangle.tga && time ./main


#include<bits/stdc++.h>
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


struct Point{
    int x,y;
};

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

bool check(Point p,Point a,Point b,Point c){
    // create three lines 
    /*
     create line AB and then check sign of point C and point P && BC && CA
    */
    float m = (b.y - a.y)/static_cast<float>(b.x-a.x);
    float d = a.y - (m * a.x); 

    // line is y - mx - d

    return ((p.y - m * p.x -d) * (c.y-m*c.x -d) > 0);
    
}


void triangle(Point &a,Point &b,Point &c,TGAImage &framebuffer,TGAColor color){

    line(a.x,a.y,b.x,b.y,framebuffer,color);
    line(b.x,b.y,c.x,c.y,framebuffer,color);
    line(c.x,c.y,a.x,a.y,framebuffer,color);

    // fill triangle

    int mn_x = min({a.x,b.x,c.x});
    int mx_x = max({a.x,b.x,c.x});

    int mn_y = min({a.y,b.y,c.y});
    int mx_y = max({a.y,b.y,c.y});

    for(int x = mn_x;x<=mx_x;x++){
        for(int y =mn_y;y<=mx_y;y++){
            Point p(x,y);
            if(check(p,a,b,c) && check(p,b,c,a) && check(p,a,c,b)){
                framebuffer.set(p.x,p.y,color);
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

    Point a(ax,ay);
    Point b(bx,by);
    Point c(cx,cy);
    Point d(dx,dy);

    triangle(b,a,c,framebuffer,red);

    triangle(d,a,b,framebuffer,white);

    framebuffer.write_tga_file("triangle.tga");
    return 0;
}
