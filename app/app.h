#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_synth.h"
#include "./app_view_synth.h"
#include <app_core_display.h>
#include <app_core_renderer.h>

#include <PdBase.hpp>

class App {
public:
    App_Synth synth;

    App_Display* display;
    UiKeys keys;
    pd::PdBase* pd;

    App_View_Synth synthView;

    bool rendered = false;

    App(App_Display* _display, pd::PdBase* _pd)
        : display(_display)
        , pd(_pd)
        , synthView(&synth)
    {
    }

    int16_t sample()
    {
        return synth.sample();
    }

    void render()
    {
        if (display->ready()) {
            // TODO find a better place way to reset cursor
            display->reset();
            synthView.render(display);
            display->drawText();
            rendered = true;
        }
    }

    void handleUi(uint16_t keysBin)
    {
        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.Edit = (keysBin >> UI_KEY_EDIT) & 1;
        keys.Menu = (keysBin >> UI_KEY_MENU) & 1;
        keys.Action = (keysBin >> UI_KEY_ACTION) & 1;

        // SDL_Log("%d%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.Edit, keys.Menu, keys.Action);

        // might need to move the logic?
        // if (keys.Action) {
        //     printf("Action on\n");
        //     synth.adsr[0].on();
        // } else if (synth.adsr[0].isOn()) {
        //     printf("Action off\n");
        //     synth.adsr[0].off();
        // }
        if (keys.Action) {
            printf("Action on\n");
            pd->sendNoteOn(1, 60, 100);
            pd->sendFloat("noteFromCpp", 60);
            pd->sendFloat("veloFromCpp", 100);
        } else {
            pd->sendNoteOn(1, 60, 0);
            pd->sendFloat("noteFromCpp", 60);
            pd->sendFloat("veloFromCpp", 0);
        }

        if (synthView.update(&keys, display) != VIEW_NONE) {
            render();
        }
    }

    void quit()
    {
        // App_Renderer renderer;
        // for (uint8_t i; i < APP_MENU_SIZE; i++) {
        //     if (menu[i].view) {
        //         renderer.reset();
        //         renderer.startRow = 0;
        //         menu[i].view->snapshot(&renderer);
        //     }
        // }
    }

    void start()
    {
        // for (uint8_t i; i < APP_MENU_SIZE; i++) {
        //     if (menu[i].view) {
        //         menu[i].view->loadSnapshot();
        //     }
        // }
    }
};

#endif