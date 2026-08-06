# TinyRenderer Study Notes (Part 1)

## 1. Goal

Before understanding line drawing or rasterization, understand **how an
image is represented in memory**.

TinyRenderer first creates a **framebuffer** (an image in RAM). Drawing
algorithms simply modify pixels in this framebuffer. At the end, the
framebuffer is written to `framebuffer.tga`.

------------------------------------------------------------------------

# 2. What is an Image?

An image is a **rectangular grid of pixels**.

    +---+---+---+
    |   |   |   |
    +---+---+---+
    |   |   |   |
    +---+---+---+
    |   |   |   |
    +---+---+---+

Each square is called a **pixel** (Picture Element).

A pixel stores a color.

------------------------------------------------------------------------

# 3. Image Dimensions

``` cpp
constexpr int width = 64;
constexpr int height = 64;
```

means

-   64 columns
-   64 rows

Total pixels:

    64 × 64 = 4096 pixels

------------------------------------------------------------------------

# 4. Pixel Color

TinyRenderer uses

    Blue
    Green
    Red
    Alpha

(BGRA order)

Each channel occupies **1 byte**.

RGB image:

    3 bytes / pixel

RGBA image:

    4 bytes / pixel

Alpha represents opacity.

    255 = fully opaque
    0   = fully transparent

------------------------------------------------------------------------

# 5. Memory Required

For an RGB image

    4096 pixels × 3 bytes

    = 12288 bytes
    ≈ 12 KB

------------------------------------------------------------------------

# 6. The Biggest Idea

Although we think of an image as a 2D grid, computers store memory as
**one long array of bytes**.

Instead of

    P00 P01 P02
    P10 P11 P12
    P20 P21 P22

memory contains

    P00 P01 P02 P10 P11 P12 P20 P21 P22

This is called **row-major order**.

------------------------------------------------------------------------

# 7. Mapping Coordinates to Memory

The key formula is

    pixel_index = y * width + x

For RGB images

    byte_index = (y * width + x) * bytes_per_pixel

TinyRenderer later uses exactly this idea inside `set()`.

------------------------------------------------------------------------

# 8. Matrix Coordinates vs Graphics Coordinates

This was the biggest source of confusion.

## Matrix notation

    (row, column)

Example

    (7,3)

    ↓

    Row = 7
    Column = 3

    Index

    7 * width + 3

## Graphics notation

TinyRenderer uses

    (x,y)

where

    x = column
    y = row

Example

    (7,3)

    ↓

    Column = 7
    Row = 3

    Index

    3 * width + 7

**Mental translation while reading TinyRenderer**

``` cpp
framebuffer.set(x,y,color);
```

Think of it as

    framebuffer.set(column,row,color);

------------------------------------------------------------------------

# 9. Coordinate System

TinyRenderer conceptually uses

              y
              ↑
              |
              |
    ----------+--------→ x

The logical graphics origin is at the **bottom-left**.

Later we will see that image file formats sometimes store rows
differently; `write_tga_file()` handles this.

------------------------------------------------------------------------

# 10. BGRA

TinyRenderer stores colors as

    Blue
    Green
    Red
    Alpha

instead of

    Red
    Green
    Blue
    Alpha

This is simply a storage convention used by the TGA format and some
graphics APIs.

------------------------------------------------------------------------

# 11. Important Gotchas

### Gotcha 1

Graphics uses `(x,y)`.

Matrices use `(row,column)`.

Don't mix them.

------------------------------------------------------------------------

### Gotcha 2

`x` is **horizontal**.

`y` is **vertical**.

------------------------------------------------------------------------

### Gotcha 3

The framebuffer is **not the screen**.

It is just a block of memory.

Later,

``` cpp
framebuffer.write_tga_file("framebuffer.tga");
```

writes that memory into an image file.

------------------------------------------------------------------------

### Gotcha 4

The image is **not stored as a 2D array**.

It is stored as a flat 1D byte array.

------------------------------------------------------------------------

### Gotcha 5

The most important formula in this chapter:

    pixel_index = y * width + x

Memorize this.

It appears throughout graphics programming.

------------------------------------------------------------------------

# 12. Current Execution Flow

    main()

    ↓

    TGAImage framebuffer(64,64,RGB)

    ↓

    Constructor allocates memory

    ↓

    line()

    ↓

    framebuffer.set()

    ↓

    Pixels in memory change

    ↓

    write_tga_file()

    ↓

    framebuffer.tga

------------------------------------------------------------------------

# Next Topic

Study `tgaimage.h` beginning with:

-   `#pragma once`
-   `#pragma pack(push,1)`
-   `TGAHeader`
-   Why image files need headers
-   How a `.tga` file stores metadata
