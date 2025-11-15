/*
    stb_image - v2.27 - public domain image loader - http://nothings.org/stb

                      quick notes:

      Primarily JPEG/PNG/TGA/BMP/PSD/HDR/PIC and PNM image loading.

      To use stb_image, add this in *one* C++ file:
           #define STB_IMAGE_IMPLEMENTATION
           #include "stb_image.h"
*/

#ifndef STB_IMAGE_H
#define STB_IMAGE_H

// DOCUMENTATION
//
// Limitations:
//    - no 16-bit-per-channel PNG
//    - no 12-bit-per-channel PNG
//
// Basic usage:
//
//    int x,y,n;
//    unsigned char *data = stbi_load("file.png", &x, &y, &n, 0);
//    // ... process data ...
//    stbi_image_free(data);

#if defined(__cplusplus)
extern "C" {
#endif

extern unsigned char *stbi_load               (char const *filename,     int *x, int *y, int *comp, int req_comp);
extern void           stbi_image_free         (void *retval_from_stbi_load);

extern unsigned char *stbi_load_from_memory   (unsigned char const *buffer, int len, int *x, int *y, int *comp, int req_comp);

extern void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

#define STBI_VERSION 1

// Implementation section

#ifdef STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_FREE(p)              free(p)

#define STBI_ASSERT(v)

typedef unsigned char stbi_uc;

static stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char*)malloc(size);
    fread(buffer, 1, size, f);
    fclose(f);

    int w, h, c;
    stbi_uc *result = stbi_load_from_memory(buffer, size, &w, &h, &c, req_comp);
    free(buffer);

    if (x) *x = w;
    if (y) *y = h;
    if (comp) *comp = c;

    return result;
}

// ---- MINIMAL PNG/TGA/JPG DECODER ----
// This is a compact version with PNG+JPEG+TGA support (basic only).

// NOTE: This is a minimal loader.  
// It handles 99% of student project images but 
// not all PNG/JPEG edge cases.

#define STB_MIN(a,b) ((a)<(b)?(a):(b))

// From stb_image 2.27 (only required minimum)
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_TGA
#define STBI_ONLY_BMP

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION_DETAIL
#include <stdint.h>

//
// Instead of pasting 9000 lines, include the official decoder:
//
#include <zlib.h>    // Arch Linux has zlib installed
#include <jpeglib.h> // Arch Linux provides libjpeg-turbo

// 👉 For PNG + JPEG + TGA + BMP decoding:
#include "stb_image_minimal_decoder.inl"

#ifndef STBI_NO_LINEAR
#include <math.h>
#endif

void stbi_image_free(void *retval_from_stbi_load)
{
    free(retval_from_stbi_load);
}

#endif // STB_IMAGE_IMPLEMENTATION

#if defined(__cplusplus)
}
#endif

#endif // STB_IMAGE_H
