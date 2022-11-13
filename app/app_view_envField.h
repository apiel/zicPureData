#ifndef APP_VIEW_ENV_FIELD_H_
#define APP_VIEW_ENV_FIELD_H_

#include <app_core_view_table.h>

#include <zic_mod_adsr.h>

class App_View_EnvField : public App_View_TableField {
protected:
    uint8_t envId;
    Zic_Mod_Adsr* adsr;

public:
    App_View_EnvField(uint8_t _envId, Zic_Mod_Adsr* _adsr)
        : envId(_envId)
        , adsr(_adsr)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        uint8_t cursor = 0;
        switch (col) {
        case 0:
            sprintf(renderer->text + strlen(renderer->text), "%d", envId + 1);
            break;

        case 1:
            cursor = 4;
            sprintf(renderer->text + strlen(renderer->text), " A=%-4d", adsr->getAttack());
            break;

        case 2:
            cursor = 4;
            sprintf(renderer->text + strlen(renderer->text), " D=%-4d", adsr->getDecay());
            break;

        case 3:
            cursor = 3;
            sprintf(renderer->text + strlen(renderer->text), " S=%-3d", (uint8_t)(adsr->getSustain() * 100));
            break;

        case 4:
            cursor = 5;
            sprintf(renderer->text + strlen(renderer->text), " R=%-5d", adsr->getRelease());
            break;

        default:
            break;
        }
        if (selectedRow == row && selectedCol == col) {
            renderer->setCursor(cursor, -cursor);
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        switch (col) {
        case 1: 
            adsr->setAttack(adsr->getAttack() + keys->getDirection(10.0f));
            return VIEW_CHANGED;
        case 2:
            adsr->setDecay(adsr->getDecay() + keys->getDirection(10.0f));
            return VIEW_CHANGED;
        case 3:
            adsr->setSustain(adsr->getSustain() + keys->getDirection(0.01f));
            return VIEW_CHANGED;
        case 4:
            adsr->setRelease(adsr->getRelease() + keys->getDirection(10.0f));
            return VIEW_CHANGED;
        default:
            break;
        }
        return VIEW_NONE;
    }
};

#endif