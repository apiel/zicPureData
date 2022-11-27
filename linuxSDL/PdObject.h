#pragma once

#include "PdBase.hpp"

#include <iostream>

using namespace std;
using namespace pd;

// custom receiver class
class PdObject : public pd::PdReceiver, public pd::PdMidiReceiver {

public:
    // pd message receiver callbacks
    void print(const std::string& message)
    {
        cout << message << endl;
    }

    void receiveBang(const std::string& dest)
    {
        cout << "CPP: bang " << dest << endl;
    }

    void receiveFloat(const std::string& dest, float num)
    {
        cout << "CPP: float " << dest << ": " << num << endl;
    }

    void receiveSymbol(const std::string& dest, const std::string& symbol)
    {
        cout << "CPP: symbol " << dest << ": " << symbol << endl;
    }

    void receiveList(const std::string& dest, const pd::List& list)
    {
        cout << "CPP: list " << dest << ": " << list << endl;
    }

    void receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list)
    {
        cout << "CPP: message " << dest << ": " << msg << " " << list.toString() << " " << list.types() << endl;
    }

    // pd midi receiver callbacks
    void receiveNoteOn(const int channel, const int pitch, const int velocity)
    {
        cout << "CPP MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
    }

    void receiveControlChange(const int channel, const int controller, const int value)
    {
        cout << "CPP MIDI: control change: " << channel << " " << controller << " " << value << endl;
    }

    void receiveProgramChange(const int channel, const int value)
    {
        cout << "CPP MIDI: program change: " << channel << " " << value << endl;
    }

    void receivePitchBend(const int channel, const int value)
    {
        cout << "CPP MIDI: pitch bend: " << channel << " " << value << endl;
    }

    void receiveAftertouch(const int channel, const int value)
    {
        cout << "CPP MIDI: aftertouch: " << channel << " " << value << endl;
    }

    void receivePolyAftertouch(const int channel, const int pitch, const int value)
    {
        cout << "CPP MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
    }

    void receiveMidiByte(const int port, const int byte)
    {
        cout << "CPP MIDI: midi byte: " << port << " " << byte << endl;
    }
};
