#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pd_api.h"


static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInit)
    {
        const char* err;
        font = pd->graphics->loadFont(fontpath, &err);

        if (font == NULL) pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

        pd->display->setRefreshRate(50);

        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

const char* text = "HELLO Playdate WORLD!";
const float d2r = (M_PI * 2.0f) / 180.0f;
float angle = 0.0f;
const float seconds = 2.0f;

static int update(void* userdata)
{
    PlaydateAPI* pd = userdata;

    pd->graphics->clear(kColorWhite);
    pd->graphics->setFont(font);

    int cx = pd->display->getWidth() / 2;
    int cy = pd->display->getHeight() / 2;
    int tw = pd->graphics->getTextWidth(font, text, strlen(text), kASCIIEncoding, 0) / 2;

    int x = cx + (int)(cos(angle * d2r) * 32);
    int y = cy - (int)(sin(angle * d2r) * 32);
    pd->graphics->drawText(text, strlen(text), kASCIIEncoding, x - tw, y);

    angle += ((360.0f / seconds) / 50.0f);

#ifndef NDEBUG
    pd->system->drawFPS(0, 0);
#endif

    return 1;
}

