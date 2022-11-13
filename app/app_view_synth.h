#ifndef APP_VIEW_SYNTH_H_
#define APP_VIEW_SYNTH_H_

#include <app_core_view_table.h>
// #include <zic_wave_file.h>

#include "./app_synth.h"
#include "./app_view_envField.h"
#include "./app_view_wavetableField.h"
#include "./app_view_filterField.h"

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 5

class App_View_Synth : public App_View_Table {
protected:
    App_View_WavetableField wavetable1Field;
    App_View_WavetableField wavetable2Field;
    App_View_WavetableField wavetable3Field;
    App_View_WavetableField wavetable4Field;

    App_View_EnvField env1Field;
    App_View_EnvField env2Field;

    App_View_FilterField filterField;

    App_View_TableField* fields[VIEW_INSTR_ROW * VIEW_INSTR_COL] = {
        // clang-format off
        &wavetable1Field, &wavetable1Field, &wavetable1Field, &wavetable1Field, NULL,
        &wavetable2Field, &wavetable2Field, &wavetable2Field, &wavetable2Field, NULL,
        &wavetable3Field, &wavetable3Field, &wavetable3Field, &wavetable3Field, NULL,
        &wavetable4Field, &wavetable4Field, &wavetable4Field, &wavetable4Field, NULL,
        &env1Field, &env1Field, &env1Field, &env1Field, &env1Field,
        &env2Field, &env2Field, &env2Field, &env2Field, &env2Field,
        &filterField, &filterField, &filterField, &filterField, NULL,
        NULL, NULL, NULL, NULL, NULL,
        // clang-format on
    };

public:
    App_View_Synth(App_Synth* _synth)
        : App_View_Table(fields, VIEW_INSTR_ROW, VIEW_INSTR_COL)
        , wavetable1Field(0, &_synth->wavetable[0])
        , wavetable2Field(1, &_synth->wavetable[1])
        , wavetable3Field(2, &_synth->wavetable[2])
        , wavetable4Field(3, &_synth->wavetable[3])
        , env1Field(0, &_synth->adsr[0])
        , env2Field(1, &_synth->adsr[1])
        , filterField(&_synth->filter)
    {
        initSelection();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColor(0, 6, 0, 1);
        // renderer->useFirstLetterHilighted();
        App_View_Table::initDisplay(renderer);
    }
};

#endif