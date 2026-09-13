#ifndef ENTITIES_H
#define ENTITIES_H

#include "hdw-touch.h"
#include "touchUtils.h"

#include "entities_structs.h"
#include "fox_library.h"

static void debugTouch(void) {
    linearTouch_t touches[2] = {0};

    uint8_t count = getTouchLinear(touches, 2);

    printf(
        "count=%u | RIGHT[0]: touched=%d pos=%d intensity=%d | "
        "LEFT[1]: touched=%d pos=%d intensity=%d\n",
        count,

        touches[0].touched,
        touches[0].position,
        touches[0].intensity,

        touches[1].touched,
        touches[1].position,
        touches[1].intensity
    );
}

static bool hadTouchX = false;
static bool hadTouchY = false;
static int32_t oldX = 0;
static int32_t oldY = 0;
static void updateTouchCamera(Camera_t *cam) {
    linearTouch_t touch[2] = {0};
    getTouchLinear(touch, ARRAY_SIZE(touch));
    float rotSpd = 0.0025f;

    if (touch[0].touched) {
        int32_t x = touch[0].position;

        if (hadTouchX) {
            int32_t dx = x - oldX;
            cam->rot.y += dx * rotSpd;

            if (cam->rot.x > DEG2RAD(90.0f)) cam->rot.x = DEG2RAD(90.0f);
            if (cam->rot.x < DEG2RAD(-90.0f)) cam->rot.x = DEG2RAD(-90.0f);
        }

        oldX = x;
        hadTouchX = true;
    } else {
        hadTouchX = false;
    }

    if (touch[1].touched) {
        int32_t y = touch[1].position;

        if (hadTouchY) {
            int32_t dy = y - oldY;
            cam->rot.x += dy * rotSpd;

            if (cam->rot.y < DEG2RAD(0.0f)) cam->rot.y += DEG2RAD(360.0f);
            if (cam->rot.y > DEG2RAD(360.0f)) cam->rot.y -= DEG2RAD(360.0f);
        }

        oldY = y;
        hadTouchY = true;
    } else {
        hadTouchY = false;
    }

    printf("T0 touched=%d pos=%d | T1 touched=%d pos=%d\n", touch[0].touched, touch[0].position, touch[1].touched, touch[1].position);
}

static buttonState_t button_inputs() {
     static buttonState_t state = {
        .up = false,
        .down = false,
        .left = false,
        .right = false,
        .a = false,
        .b = false
    };

    buttonEvt_t evt;
    while (checkButtonQueueWrapper(&evt)) {
        if (evt.button == PB_UP)    state.up    = evt.down;
        if (evt.button == PB_DOWN)  state.down  = evt.down;
        if (evt.button == PB_LEFT)  state.left  = evt.down;
        if (evt.button == PB_RIGHT) state.right = evt.down;

        if (evt.button == PB_A)     state.a = evt.down;
        if (evt.button == PB_B)     state.b = evt.down;
    }

    return state;
}

static void move_camera(Camera_t *cam, float dt, buttonState_t buttonState) {
    float yaw = cam->rot.y;
    float moveSpd = 5.0f * dt;
    
    if (!buttonState.a) {
        if (buttonState.up) {
            cam->pos.x += moveSpd * sin(yaw);
            cam->pos.z += moveSpd * cos(yaw);
        } if (buttonState.down) {
            cam->pos.x -= moveSpd * sin(yaw);
            cam->pos.z -= moveSpd * cos(yaw);
        } if (buttonState.left) {
            cam->pos.x -= moveSpd * cos(yaw);
            cam->pos.z += moveSpd * sin(yaw);
        } if (buttonState.right) {
            cam->pos.x += moveSpd * cos(yaw);
            cam->pos.z -= moveSpd * sin(yaw);
        }
    } else {
        if (buttonState.up) { cam->pos.y += moveSpd; }
        if (buttonState.down) { cam->pos.y -= moveSpd; }
    }
}

#endif