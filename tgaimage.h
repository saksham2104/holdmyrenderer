#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1)
struct TGAHeader { // represents the TGA file header i.e the metadata of the image file
    std::uint8_t  idlength = 0; //
    std::uint8_t  colormaptype = 0;//
    std::uint8_t  datatypecode = 0;//
    std::uint16_t colormaporigin = 0;
    std::uint16_t colormaplength = 0;
    std::uint8_t  colormapdepth = 0;
    std::uint16_t x_origin = 0;// 
    std::uint16_t y_origin = 0;//
    std::uint16_t width = 0;//
    std::uint16_t height = 0;// 
    std::uint8_t  bitsperpixel = 0;
    std::uint8_t  imagedescriptor = 0;//
};
#pragma pack(pop)

struct TGAColor { // represents a pixel in the image
    std::uint8_t bgra[4] = {0,0,0,0}; // blue, green, red, alpha(transparency) channels
    std::uint8_t bytespp = 4; //bytes per pixel , in rgb just ignore the alpha channel , in grayscale just use the first channel
    std::uint8_t& operator[](const int i) { return bgra[i]; } // operator overload to access the pixel values , instead of writing color.bgra[0] we can write color[0] (TGAColor color is the object )
};

struct TGAImage { 
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };
    TGAImage() = default;
    TGAImage(const int w, const int h, const int bpp);
    bool  read_tga_file(const std::string filename);
    bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;
    void flip_horizontally();
    void flip_vertically();
    TGAColor get(const int x, const int y) const;
    void set(const int x, const int y, const TGAColor &c);
    int width()  const;
    int height() const;
private:
    bool load_rle_data(std::ifstream &in); // will study later
    bool unload_rle_data(std::ofstream &out) const; // will study later
    int w = 0, h = 0; 
    std::uint8_t bpp = 0; 
    std::vector<std::uint8_t> data = {}; // contains the entire image , we know by bpp how many bytes are used for each pixel i.e 3 or 4
};

/*

y* width + x -> takes us to the exact pixel 

bpp - how many pixels in an image i.e if bpp = 3 , 3 consecutive entries in an array represent one pixel
so suppose i want to go to (7,3) point i.e width = 7 height = 3

start from (0,0) 

data arr(visualisation) : data[0],data[1],data[2] , data[3] , data[4] , data[5]      .... data [63] -> 64 entries
                                                  (0,1)                               (0,21)
                          data[64], data[65] , data[66] , data[67],data[68],data[69]
                            (1,0)
            so (1,1) means -> one entire row of pixels i.e 64 entries + skip 1




*/ 

