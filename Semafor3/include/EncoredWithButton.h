#ifndef ENCODER_WITH_BUTON_H
#define ENCODER_WITH_BUTON_H

#include <ESP32Encoder.h>

class EncoredWithButton : public ESP32Encoder {

    int32_t lastState = 0;
    uint8_t buttonPin;

public:

    void attachButtonPin(uint8_t buttonPin);

    int8_t wasTurned();

    bool wasClicked();
};

#endif