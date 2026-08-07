//g++-16 -std=c++23 main.cpp tgaimage.cpp -o main && ./main && open main.tga 

//  time ./main -> to compare time

#include<bits/stdc++.h>
#include <fstream>
#include <string>
#include "tgaimage.h"

using namespace std;

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

constexpr int width  = 800;
constexpr int height = 800;


struct Point{
    int x,y;
    int z = 0;
};

struct Vertice {
    float x, y, z; // coordinates bw 0 to 1
};

struct Face {
    int a, b, c; // indice of Vertices (1 - indexed)
};

std::vector<Vertice> vertices;
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
            Vertice v;

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
    /*
     create line AB and then check sign of point C and point P && BC && CA

     this is a cheap trick , will implement barycentric coordinates later
    */
    float m = (b.y - a.y)/static_cast<float>(b.x-a.x);
    float d = a.y - (m * a.x); 

    // line is y - mx - d

    return ((p.y - m * p.x -d) * (c.y-m*c.x -d) > 0);
    
}


double area_triangle(Point &a,Point &b,Point &c){
    return (0.5) * ((b.y - a.y)*(b.x + a.x) + (c.y-b.y) * (c.x + b.x) + (a.y - c.y) * (c.x + a.x));
}


void triangle(Point &a,Point &b,Point &c,TGAImage&zbuffer,TGAImage &framebuffer,TGAColor color){

    // line(a.x,a.y,b.x,b.y,framebuffer,color);
    // line(b.x,b.y,c.x,c.y,framebuffer,color);
    // line(c.x,c.y,a.x,a.y,framebuffer,color);

    // fill triangle

    int mn_x = min({a.x,b.x,c.x});
    int mx_x = max({a.x,b.x,c.x});

    int mn_y = min({a.y,b.y,c.y});
    int mx_y = max({a.y,b.y,c.y});

    double area = area_triangle(a,b,c);

    for(int x = mn_x;x<=mx_x;x++){
        for(int y =mn_y;y<=mx_y;y++){
            Point p(x,y);
            double alpha =  area_triangle(p,b,c) / area;
            double beta  =  area_triangle(p,c,a) / area;
            double gamma =  area_triangle(p,a,b) / area;

            if(alpha < 0 || beta < 0 || gamma < 0) continue;

            unsigned char z = static_cast<unsigned char>(alpha * a.z + beta * b.z + gamma * c.z);
            p.z = z;

            if(z < zbuffer.get(x,y)[0]) continue;

            framebuffer.set(x,y,color);
            
            zbuffer.set(x,y,{z});
  
        }
    }
}


Point convert(Vertice v){
    int x = (v.x + 1)* (width) / 2;
    int y = (v.y + 1) * (height)/2;
    int z = (v.z + 1) * (255) / 2;

    return Point(x,y,z);
}


int main(int argc, char** argv) {

    //std::srand(std::time(nullptr));

    TGAImage framebuffer(width, height, TGAImage::RGB);

    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    read_obj_file();

    std::cout << "Vertices: " << vertices.size() << '\n';
    std::cout << "Faces: " << faces.size() << '\n';

    for (const Face &f : faces) {

        Vertice v0 = vertices[f.a];
        Vertice v1 = vertices[f.b];
        Vertice v2 = vertices[f.c];

        Point a = convert(v0);
        Point b = convert(v1);
        Point c = convert(v2);

        TGAColor rnd;

        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;

        triangle(a,b,c,zbuffer,framebuffer,rnd);

    }

    framebuffer.write_tga_file("main.tga");  
    zbuffer.write_tga_file("zbuffer.tga"); 
    return 0;

}
