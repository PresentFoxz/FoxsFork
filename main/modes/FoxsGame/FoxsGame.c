#include "FoxsGame.h"
#include "esp_random.h"

double xVel = 0; double yVel = 0;
double x = 0; double y = 0; 
int16_t falling = 0;
int16_t count = 0; uint16_t btnState;


static const char foxName[]  = "Fox";

/*
static void foxEnterMode(void);
static void foxExitMode(void);
*/

static void foxMainLoop(int64_t elapsedUs);


/*
static void foxBackgroundDrawCallback(int16_t x, int16_t y, int16_t w, int16_t h, int16_t up, int16_t upNum);
*/

swadgeMode_t foxMode = {
    .modeName                 = foxName,
    .wifiMode                 = ESP_NOW,
    .overrideUsb              = false,
    .usesAccelerometer        = false,
    .usesThermometer          = false,
    .overrideSelectBtn        = false,
    .fnMainLoop               = foxMainLoop
};

/*
static void foxEnterMode(void)
{

}
 
static void foxExitMode(void)
{
}
*/

static void draw()
{
    int16_t xPos = (int)x;
    int16_t yPos = (int)y;
    fillDisplayArea(0, 0, 280, 240, c554);
    fillDisplayArea(xPos, yPos, 45 + xPos, 45 + yPos, c050);
}

static void movement(double speed, double friction, double g)
{
    buttonEvt_t evt;
    while (checkButtonQueueWrapper(&evt))
    {
        btnState = evt.state;
        printf("state: %04X, button: %d, down: %s\n",
               evt.state, evt.button, evt.down ? "down" : "up");
    }

    if (xVel > 0)
    {
        xVel -= friction;
        if (xVel < 0)
            xVel = 0;
    }
    else if (xVel < 0)
    {
        xVel += friction;
        if (xVel > 0)
            xVel = 0;
    }
    if (xVel > 3){
        xVel = 3;
    }
    else if (xVel < -3){
        xVel = -3;
    }

    if (btnState & PB_RIGHT)
    {
        xVel += speed;
    }
    if (btnState & PB_LEFT)
    {
        xVel -= speed;
    }
    if (btnState & PB_A && falling < 2)
    {
        y -= 1;
        yVel = -8.2;
    }

    if (btnState & PB_B)
    {
        x = 0;
        y = 0;
        xVel = 0;
        yVel = 0;
    }

    falling++;
    if (y >= 100)
    {
        y = 100;
        yVel = 0;
        falling = 0;
    }
    else
    {
        yVel += g;
    }

    y += yVel;
    x += xVel;
}

static void foxMainLoop(int64_t elapsedUs)
{
    movement(0.8, 0.3, 0.8);
    draw();
}