#include "Arduino.h"
#include "IO-Kit.h"

Output::Output(int pin) {
    this->_pinNumber = pin;
    pinMode(this->_pinNumber, OUTPUT);
}

void Output::set(bool val) {
    digitalWrite(this->_pinNumber, val);
    this->_pinStatus = val * 255;
}

void Output::setPWM(int val) {
    analogWrite(this->_pinNumber, val);
    this->_pinStatus = val;
}

double Output::operator=(double val) {
    val = constrain(val, 0.0, 1.0);
    if (val == 1.0 || val == 0.0) {
        this->set(bool(val));
    } else {
        this->setPWM(round(val * 255));
    }
    return val;
}

int Output::operator<<=(int val) {
    this->setPWM(val);
    return val;
}

Output::operator double() {
    return _pinStatus / 255.0;
}

byte Output::raw(void) {
    return this->_pinStatus;
}