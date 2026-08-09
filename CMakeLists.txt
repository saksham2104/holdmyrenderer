cmake_minimum_required(VERSION 3.20)

project(TinyRenderer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tinyrenderer
    main.cpp

    Image/tgaimage.cpp
    Model/model.cpp
    Rasterizer/rasterizer.cpp
)

target_include_directories(tinyrenderer PRIVATE
    geometry
    Image
    Model
    Rasterizer
)