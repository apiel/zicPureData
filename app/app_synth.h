#ifndef APP_SYNTH_H_
#define APP_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./app_wavetable.h"

#define APP_SYNTH_WAVE_COUNT 4
#define APP_SYNTH_ENV_COUNT 2
#define APP_SYNTH_PATCHE_COUNT 4

class App_Synth_Patch {
public:
    uint8_t out = 0;
    uint8_t outPos = 0;
    uint8_t target = 0;
    uint8_t targetPos = 0;
};

class App_Synth_OscState {
public:
    bool outIsFree = true;
    int16_t out = 0;

    int16_t pitch = 0;
    float morph = 0.0f;
    int16_t amplitude = 0;
};

class App_Synth_EnvState {
public:
    bool outIsFree = true;
    float out = 0.0f;
};

class App_Synth {
protected:
    enum {
        NO_OUT,
        OSC_OUT,
        ENV_OUT,
        PITCH_OUT,
    };

    enum {
        OSC_PITCH,
        OSC_MORPH,
        OSC_AMPLITUDE,
        ENV_ATTACK,
        ENV_DECAY,
        ENV_SUSTAN,
        ENV_RELEASE,
        FILTER_INPUT,
        FILTER_CUTOFF,
        FILTER_RESONANCE,
        OUTPUT,
    };

    App_Synth_OscState oscState[APP_SYNTH_WAVE_COUNT];
    App_Synth_EnvState envState[APP_SYNTH_ENV_COUNT];

    int16_t filterOut = 0;
    int16_t filterInput = 0;

    void outToInput(float out, App_Synth_Patch* patch)
    {
        switch (patch->target) {
        case OSC_PITCH:
            oscState[patch->targetPos].pitch = out * 4096.0f; // why 4096.0f ?? because copilot said it :p
            break;
            
        case OSC_MORPH:
            oscState[patch->targetPos].morph = out;
            break;

        case OSC_AMPLITUDE:
            oscState[patch->targetPos].amplitude = out * 100;
            break;
        }
    }

    void outToInput(int16_t out, App_Synth_Patch* patch)
    {
        outToInput(out / 32768.0f, patch);
    }

public:
    App_Wavetable wavetable[APP_SYNTH_WAVE_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr[APP_SYNTH_ENV_COUNT];

    App_Synth_Patch patches[APP_SYNTH_PATCHE_COUNT] = {
        // { ENV_OUT, 0, OSC_AMPLITUDE, 0 },
        // { OSC_OUT, 0, FILTER_INPUT, 0 },
        // { NO_OUT, 0, 0 , 0 },
        // { NO_OUT, 0, 0 , 0 }
    };

    int16_t sample()
    {
        for (int i = 0; i < APP_SYNTH_WAVE_COUNT; i++) {
            oscState[i].outIsFree = true;
        }

        for (int i = 0; i < APP_SYNTH_ENV_COUNT; i++) {
            envState[i].outIsFree = true;
        }

        for (uint8_t i = 0; i < APP_SYNTH_WAVE_COUNT; i++) {
            uint8_t outPos = patches[i].outPos;
            switch (patches[i].out) {
            case OSC_OUT: {
                if (oscState[outPos].outIsFree) {
                    oscState[outPos].out = wavetable[outPos].next();
                    oscState[outPos].outIsFree = false;
                }
                outToInput(oscState[outPos].out, &patches[i]);
                break;
            }

            case ENV_OUT: {
                if (envState[outPos].outIsFree) {
                    envState[outPos].out = adsr[outPos].next();
                    envState[outPos].outIsFree = false;
                }
                outToInput(envState[outPos].out, &patches[i]);
                break;
            }

            default:
                break;
            }
        }

        filterInput = oscState[0].out * envState[0].out;
        filterOut = filter.next(filterInput);

        return filterOut;
    }
};

// the mixer should be done in the part while assigning patches
//
// osc in lfo mode could be run in one shot, for example for kick design
// therefor would need a way to design custom wavetable :p
// maybe we should think to use linear interpolation for the wavetable to have smoother transitions

// Use Y button to trigger noteOn noteOff
// L1/R1 could be to switch the note to play?? or not :p

// would be great that at the end of a phase of the OSC to trigger a something
// e.g. trigger a random pitch change
// or trigger pitch inc?

#endif