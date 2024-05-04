#include "FoxsGame.h"
#include <math.h>

static const char foxName[] = "Fox";
static void foxMainLoop(int64_t elapsedUs);

uint16_t btnState;
bool runner = true;

/*
Player variables
*/
int x1 = 0; int y1 = 100; int x1Speed = 0; int y1Speed = 0; int state1 = 0;
int attackBoxes[2][4][4] = {
    {
        {x1, y1, x1 + 10, y1}, 
        {x1 + 10, y1, x1 + 10, y1 + 10}
        {x1 + 10, y1 + 10, x1, y1 + 10}
        {x1, y1 + 10, x1, y1}
    }
    {
        {0, 150, 280, 150}, {280, 150, 280, 240}, 
        {280, 240, 0, 240}, {0, 240, 0, 150}
    }
};

swadgeMode_t foxMode = {
    .modeName = foxName,
    .wifiMode = ESP_NOW,
    .overrideUsb = false,
    .usesAccelerometer = false,
    .usesThermometer = false,
    .overrideSelectBtn = false,
    .fnMainLoop = foxMainLoop
};

static void debug(ab){
    for(int z = 0; z < 2; z++){
        drawLineFast(ab[z][0][0], ab[z][0][1], ab[z][0][2], ab[z][0][3], c555);
        drawLineFast(ab[z][1][0], ab[z][1][1], ab[z][1][2], ab[z][1][3], c555);
        drawLineFast(ab[z][2][0], ab[z][2][1], ab[z][2][2], ab[z][2][3], c555);
        drawLineFast(ab[z][3][0], ab[z][3][1], ab[z][3][2], ab[z][3][3], c555);
    }
}

static void move() {
    buttonEvt_t evt = {0};
    while (checkButtonQueueWrapper(&evt)) {
        btnState = evt.state;
        printf("state: %04X, button: %d, down: %s\n",
               evt.state, evt.button, evt.down ? "down" : "up");
    }

    if (btnState & PB_UP && falling < 2) {
        y1Speed -= 5;
    }
    /*
    if (btnState & PB_DOWN) {
    }
    */
    if (btnState & PB_LEFT) {
        x1Speed -= 0.5;
    }
    if (btnState & PB_RIGHT) {
        x1Speed += 0.5;
    }

    if ( y1 < 150 )
    {
        y1Speed += 0.5;
        falling += 1;
    }
    else if( y1 >= 150 ){
        y1 = 150;
        y1Speed = 0;
        falling = 0;
    }

    x1 += x1Speed;
    y1 += y1Speed;
}

static void foxMainLoop(int64_t elapsedUs)
{
    setFrameRateUs(fps);

    if(runner == true){
        runner = false;
    }
    
    colors = 2;
    debug(attackBoxes);
}