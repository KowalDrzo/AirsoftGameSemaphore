#include "EncoredWithButton.h"

bool buttonPressed;
IRAM_ATTR void buttonPressHandler();

uint32_t lastClickedTime;

void EncoredWithButton::attachButtonPin(uint8_t pin) {

    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);

    attachInterrupt(buttonPin, buttonPressHandler, FALLING);
}

/********************************************************************************/

int8_t EncoredWithButton::wasTurned() {

    int8_t newState = getCount();

    if (newState > lastState) {

        lastState = newState;
        return 1;
    }
    if (newState < lastState) {

        lastState = newState;
        return -1;
    }
    else return 0;
}

/********************************************************************************/

bool EncoredWithButton::wasClicked() {

    if (buttonPressed) {

        buttonPressed = false;
        return true;
    }

    return false;
}

/********************************************************************************/

void buttonPressHandler() {

    if (millis() - lastClickedTime < 300) return;

    lastClickedTime = millis();
    buttonPressed = true;
}