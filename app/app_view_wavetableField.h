#ifndef APP_VIEW_WAVETABLE_FIELD_H_
#define APP_VIEW_WAVETABLE_FIELD_H_

#include <app_core_view_table.h>

#include "./app_wavetable.h"

class App_View_WavetableField : public App_View_TableField {
protected:
    uint8_t wavetableId;
    App_Wavetable* wavetable;

public:
    App_View_WavetableField(uint8_t _wavetableId, App_Wavetable* _wavetable)
        : wavetableId(_wavetableId)
        , wavetable(_wavetable)
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
            sprintf(renderer->text + strlen(renderer->text), "%d", wavetableId + 1);
            break;

        case 1:
            cursor = 14;
            sprintf(renderer->text + strlen(renderer->text), "%14s", wavetable->filename);
            break;

        case 2:
            cursor = 5;
            sprintf(renderer->text + strlen(renderer->text), " %5.1f", wavetable->getMorph() + 1.0f);
            // sprintf(renderer->text + strlen(renderer->text), " %-5.1f", 256.0f);
            break;

        case 3: {
            cursor = 7;
            float freq = wavetable->getFrequency();
            sprintf(renderer->text + strlen(renderer->text),
                freq >= 1000 ? " %f.0Hz" : (freq >= 100 ? " %5.1fHz" : (freq >= 10 ? " %5.2fHz" : " %5.3fHz")), freq);
            break;
        }
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
            wavetable->setNext(keys->getDirection());
            return VIEW_CHANGED;

        case 2: {
            float morph = wavetable->getMorph();
            wavetable->morph(morph + (keys->getDirection(0.1f)));
            return VIEW_CHANGED;
        }
        case 3: {
            float freq = wavetable->getFrequency();
            float step = freq < 10.0 ? 0.001 : (freq < 100.0 ? 0.01 : 0.1);
            wavetable->setFrequency(freq + (keys->getDirection(step)));
            return VIEW_CHANGED;
        }
        default:
            break;
        }
        return VIEW_NONE;
    }
};

#endif