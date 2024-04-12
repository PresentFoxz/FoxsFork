#include "FoxsGame.h"
#include <math.h>

static const char foxName[]  = "Fox";
static void foxMainLoop(int64_t elapsedUs);

uint16_t btnState; bool runner = true;

/*
int sl; int sr; int style = 0;
double pi = 3.14159265359;
*/

int fps = 1000000 / 30;

int wallAmt = 2;
double xRot1 = 0; double yRot1 = 0; double zRot1 = 0;
double xRot2 = 0; double yRot2 = 0; double zRot2 = 0;
int falling = 0;
int style = 0;
double ySpeed = 0; double xSpeed = 0; double zSpeed = 0;

char str[20];

int walls[2][3][2] = {
    {{100,200},{100,60},{200,100}},
    {{200,300},{100,50},{100,200}}
};

#define DTS 150

swadgeMode_t foxMode = {
    .modeName                 = foxName,
    .wifiMode                 = ESP_NOW,
    .overrideUsb              = false,
    .usesAccelerometer        = false,
    .usesThermometer          = false,
    .overrideSelectBtn        = false,
    .fnMainLoop               = foxMainLoop
};

typedef struct
{
    double camX, camY, camZ;
    double CamYDir, CamXDir;
}player; player P;

static void draw3D(double x1, double y1, double z1, double x2, double y2, double z2)
{
    int center_x = 140;
    int center_y = 120;

    if(z1 > 30 || z2 > 30)
    {
        drawLineFast((int)(center_x + x1*(DTS/(z1 + 0.01))),(int)(center_y + y1*(DTS/(z1 + 0.01)) - 20), (int)(center_x + x2*(DTS/(z2 + 0.01))), (int)(center_y + y1*(DTS/(z2 + 0.01)) - 20), c555);
        drawLineFast((int)(center_x + x1*(DTS/(z1 + 0.01))),(int)(center_y + y2*(DTS/(z1 + 0.01)) - 20), (int)(center_x + x2*(DTS/(z2 + 0.01))), (int)(center_y + y2*(DTS/(z2 + 0.01)) - 20), c555);

        drawLineFast((int)(center_x + x1*(DTS/(z1 + 0.01))),(int)(center_y + y1*(DTS/(z1 + 0.01)) - 20), (int)(center_x + x1*(DTS/(z1 + 0.01))), (int)(center_y + y2*(DTS/(z1 + 0.01)) - 20), c555);
        drawLineFast((int)(center_x + x2*(DTS/(z2 + 0.01))),(int)(center_y + y1*(DTS/(z2 + 0.01)) - 20), (int)(center_x + x2*(DTS/(z2 + 0.01))), (int)(center_y + y2*(DTS/(z2 + 0.01)) - 20), c555);
    }
}

static void myXRot(double x1,double z1,double x2,double z2,double dir)
{
    xRot1 = ((z1 * sinf(dir))+(x1 * cosf(dir)));
    zRot1 = ((z1 * cosf(dir))-(x1 * sinf(dir)));
    xRot2 = ((z2 * sinf(dir))+(x2 * cosf(dir)));
    zRot2 = ((z2 * cosf(dir))-(x2 * sinf(dir)));
}

static void myYRot(double y1,double z1,double y2,double z2,double dir)
{
    yRot1 = ((z1 * sinf(dir))+(y1 * cosf(dir)));
    zRot1 = ((z1 * cosf(dir))-(y1 * sinf(dir)));
    yRot2 = ((z2 * sinf(dir))+(y2 * cosf(dir)));
    zRot2 = ((z2 * cosf(dir))-(y2 * sinf(dir)));
}

static void move()
{
    buttonEvt_t evt = {0};
    while (checkButtonQueueWrapper(&evt))
    {
        btnState = evt.state;   
        printf("state: %04X, button: %d, down: %s\n",
               evt.state, evt.button, evt.down ? "down" : "up");
    }

    if(btnState & PB_START) { style = 1; } else { style = 0; }

    if(style == 0)
    {
        if(btnState & PB_A && falling < 2)
        {
            ySpeed = -3.5;
        }
        /*
        if(btnState & PB_B)
        {
            shoot();
        }
        */

        if(btnState & PB_UP)
        {
            xSpeed += 0.5*sinf(P.CamYDir);
            zSpeed += 0.5*cosf(P.CamYDir);
        }
        if(btnState & PB_DOWN)
        {
            xSpeed -= 0.5*sinf(P.CamYDir);
            zSpeed -= 0.5*cosf(P.CamYDir);
        }
        if(btnState & PB_LEFT)
        {
            xSpeed -= 0.5*cosf(P.CamYDir);
            zSpeed += 0.5*sinf(P.CamYDir);
        }
        if(btnState & PB_RIGHT)
        {
            xSpeed += 0.5*cosf(P.CamYDir);
            zSpeed -= 0.5*sinf(P.CamYDir);        
        }
    }
    if(style == 1)
    {
        if(btnState & PB_LEFT)
        {
            P.CamYDir -= 0.08;
        }
        if(btnState & PB_RIGHT)
        {
            P.CamYDir += 0.08;
        }
        if(btnState & PB_UP)
        {
            P.CamXDir += 0.08;
        }
        if(btnState & PB_DOWN)
        {
            P.CamXDir -= 0.08;
        }
    }

    if(xSpeed > 5){
        xSpeed = 5;
    }
    if(xSpeed < -5){
        xSpeed = -5;
    }

    if(zSpeed > 5){
        zSpeed = 5;
    }
    if(zSpeed < -5){
        zSpeed = -5;
    }

    xSpeed *= 0.8;
    zSpeed *= 0.8;

    P.camX += xSpeed;
    P.camY += ySpeed;
    P.camZ += zSpeed;
    falling++;
    if(P.camY >= 100){ P.camY = 100; ySpeed = 0; falling = 0; } 
    else if(P.camY < 100) { ySpeed += 0.3; }

    sprintf(str, "%f", xSpeed);
    printf("xSpeed: %s\n", str);

    sprintf(str, "%f", ySpeed);
    printf("ySpeed: %s\n", str);

    sprintf(str, "%f", zSpeed);
    printf("zSpeed: %s\n", str);
}

static void foxMainLoop(int64_t elapsedUs)
{
    setFrameRateUs(fps);

    if(runner == true){
        P.camX = 0;
        P.camY = 100;
        P.camZ = 180;
        P.CamXDir = 0;
        P.CamYDir = 0;

        font_t ibm;
        loadFont("ibm_vga8.font", &ibm, false);
        runner = false;
    }
    fillDisplayArea(0, 0, 280, 240, c000);
    fillDisplayArea(0, ((int)sinf(0 - P.CamXDir) * DTS / cosf(0 - P.CamXDir)), 280, 240, c000);

    for(int z = 0; z < wallAmt; z++)
    {
        myXRot(walls[z][0][0] - P.camX, walls[z][2][0] - P.camZ, walls[z][0][1] - P.camX, walls[z][2][1] - P.camZ, 0 - P.CamYDir);
        myYRot(walls[z][1][0] - P.camY, zRot1, walls[z][1][1] - P.camY, zRot2, 0 - P.CamXDir);
        draw3D(xRot1, yRot1, zRot1, xRot2, yRot2, zRot2);
    }

    move();
}