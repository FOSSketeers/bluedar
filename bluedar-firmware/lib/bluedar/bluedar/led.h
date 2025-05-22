#pragma once

#include <Ticker.h>
#include <Arduino.h>

namespace bluedar::led {
    static Ticker _blinker;

    void toggle();
    void toggleOn();
    void toggleOff();
    void blinkFast();
    void blinkSlow();
}