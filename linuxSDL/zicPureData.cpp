#include <APP_SDL/sdl2.h>

#include "../../libpd/cpp/PdBase.hpp"

#include "../app/app.h"
#include "../app/app_def.h"
#include <app_core_display.h>
#include <app_core_file.h>

UI_Display display;
App app(&display);

pd::PdBase lpd;
// PdObject pdObject;

void audioCallBack(void* userdata, Uint8* stream, int len)
{
    //     static union sampleTUNT {
    //         Uint8 ch[2];
    //         int16_t sample;
    //     } sampleDataU;

    //     for (int i = 0; i < len; i++) {
    //         sampleDataU.sample = app.sample();
    //         stream[i] = sampleDataU.ch[0];
    //         i++;
    //         stream[i] = sampleDataU.ch[1];

    // #if CHANNELS == 2
    //         i++;
    //         stream[i] = sampleDataU.ch[0];
    //         i++;
    //         stream[i] = sampleDataU.ch[1];
    // #endif
    //     }

    // short outBuffer[len];
    // lpd.processShort(len, NULL, (short*)stream);
    // lpd.processShort(len, NULL, outBuffer);

    // short outBuffer1;
    // lpd.processShort(1, NULL, &outBuffer1);

    // short outBuffer[1024];
    // lpd.processShort(1, NULL, &outBuffer[0]);

    lpd.processShort(len / 64, NULL, (short*)stream);
}

int main(int argc, char* args[])
{
    SDL_Log(">>>>>>> Start Zic Synth\n");

    if (!lpd.init(0, 2, SAMPLE_RATE)) {
        SDL_Log("Could not init pd\n");
        return 1;
    }
    // lpd.setReceiver(&pdObject);
    lpd.subscribe("cursor");
    lpd.computeAudio(true);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Zic",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    display.init(screenSurface);

    SDL_AudioDeviceID audioDevice = initAudio(audioCallBack);
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }

    app.start();
    app.render();
    SDL_UpdateWindowSurface(window);

    pd::Patch patch = lpd.openPatch("test.pd", "./");

    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            app.handleUi(ui.keys);
            SDL_Log("\n%s\n", display.text);
            // SDL_UpdateWindowSurface(window);
        }
        // SDL_Delay(10);
        if (app.rendered) {
            app.rendered = false;
            SDL_UpdateWindowSurface(window);
        }
        // short outBuffer1;
        // lpd.processShort(1, NULL, &outBuffer1);
        // printf("outBuffer1: %d\n", outBuffer1);
    }
    app.quit();

    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDevice);

    // lpd.closePatch();

    SDL_Quit();
    return 0;
}