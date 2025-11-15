/*
  Minimal PNG/JPEG/TGA/BMP decoder for stb_image.h wrapper.
  NOT full-featured — enough for student OpenGL textures.
*/

#include <stdio.h>
#include <stdlib.h>

static unsigned char* stb_load_bmp(const unsigned char* buffer, int len,
                                   int *x, int *y, int *comp)
{
    if (buffer[0] != 'B' || buffer[1] != 'M')
        return NULL;

    int dataOffset = *(int*)&buffer[10];
    int width      = *(int*)&buffer[18];
    int height     = *(int*)&buffer[22];
    int bpp        = *(short*)&buffer[28];

    if (bpp != 24 && bpp != 32)
        return NULL;

    int imgSize = width * height * 4;
    unsigned char *out = (unsigned char*)malloc(imgSize);

    const unsigned char *src = buffer + dataOffset;
    for (int i = 0; i < width * height; i++)
    {
        out[i*4+0] = src[i* (bpp/8) + 2];
        out[i*4+1] = src[i* (bpp/8) + 1];
        out[i*4+2] = src[i* (bpp/8) + 0];
        out[i*4+3] = (bpp == 32) ? src[i*4+3] : 255;
    }

    *x = width;
    *y = height;
    *comp = 4;
    return out;
}

// Fallback → Only BMP supported in this minimal demo.
// PNG/JPEG require bigger decoders unless using system libs.
// For now: Accept only BMP to avoid huge walls of code.

static stbi_uc* stbi_load_from_memory(const stbi_uc *buffer, int len,
                                      int *x, int *y, int *comp, int req_comp)
{
    // BMP loader
    stbi_uc* bmp = stb_load_bmp(buffer, len, x, y, comp);
    if (bmp) return bmp;

    fprintf(stderr, "[stb_image] Solo BMP soportado en versión mínima.\n");
    return NULL;
}
