#include "led.h"

namespace bluedar::led {
    void toggle(){
        digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    }

    void toggleOn(){
        _blinker.detach();
        digitalWrite(LED_BUILTIN, 1);
    }

    void toggleOff(){
        _blinker.detach();
        digitalWrite(LED_BUILTIN, 0);
    }

    void blinkFast(){
        _blinker.detach();
        _blinker.attach(0.2, toggle);
    }

    void blinkSlow(){
        _blinker.detach();
        _blinker.attach(0.8, toggle);
    }
}