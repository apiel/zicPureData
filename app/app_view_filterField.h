#ifndef APP_VIEW_FILTER_FIELD_H_
#define APP_VIEW_FILTER_FIELD_H_

#include <app_core_view_table.h>

#include <zic_effect_filter.h>

class App_View_FilterField : public App_View_TableField {
protected:
    Zic_Effect_Filter* filter;

public:
    App_View_FilterField(Zic_Effect_Filter* _filter)
        : filter(_filter)
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
        case 1:
            cursor = 8;
            // strcat(renderer->text, filter->getName());
            sprintf(renderer->text + strlen(renderer->text), "%8s", filter->getName());
            break;

        case 2:
            cursor = 7;
            sprintf(renderer->text + strlen(renderer->text), " %5dHz", filter->frequency);
            break;

        case 3:
            cursor = 4;
            // sprintf(renderer->text + strlen(renderer->text), " R=%-3d%%", (uint8_t)(filter->resonance * 100.0));
            sprintf(renderer->text + strlen(renderer->text), " R=%.2f", filter->resonance);
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
            filter->setFilterMode(filter->mode + keys->getOneDirection());
            return VIEW_CHANGED;
        case 2:
            filter->setFrequency(filter->frequency + keys->getDirection());
            return VIEW_CHANGED;
        case 3:
            filter->setResonance(filter->resonance + keys->getDirection(0.01f));
            return VIEW_CHANGED;
        default:
            break;
        }
        return VIEW_NONE;
    }
};

#endif