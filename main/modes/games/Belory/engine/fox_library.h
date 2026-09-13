#ifndef FOX_LIBRARY_H
#define FOX_LIBRARY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fox_lut.h"
#include "swadge.h"

typedef FILE FileType;
typedef paletteColor_t Pixel_t;

static inline void* fox_realloc(void* ptr, size_t size) { return realloc(ptr, size); }
static inline void* fox_malloc(size_t size) { return malloc(size); }
static inline void fox_free(void* ptr) { free(ptr); }

static int fox_fgets(char *out, int maxLen, FILE *file) {
    if (fgets(out, maxLen, file) == NULL) return 0;

    int len = strlen(out);
    if (len > 0 && out[len-1] == '\n') { out[len-1] = '\0'; }

    return len;
}

extern const int MAIN_SCREEN_W;
extern const int MAIN_SCREEN_H;
extern const int SCREEN_W;
extern const int SCREEN_H;

extern Pixel_t *screenBuffer;
extern Pixel_t *mainBuffer;
extern int interlace;
extern int interlaceAmt;
extern bool canInterlace;

#define DEG2RAD(x) ((x) * 0.0174532925f)

static inline float fastsqrt(float x) {
    union { float f; uint32_t i; } conv = { x };
    conv.i = 0x5f3759df - (conv.i >> 1);
    float y = conv.f;
    return y * (1.5f - 0.5f * x * y * y);
}

static inline int floor_div(float v, float s) {
    float d = v / s;
    int i = (int)d;

    if (d < 0.0f && d != (float)i) { i--; }
    return i;
}

#endif