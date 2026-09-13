#ifndef ENTITIES_STRUCTS_H
#define ENTITIES_STRUCTS_H

#include "fox_structs.h"

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool a;
    bool b;
} buttonState_t;

extern Camera_t cam;

#endif