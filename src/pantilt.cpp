#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ostream>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "pantilt.h"

#define REG_CONFIG 0x00
#define REG_SERVO1 0x01
#define REG_SERVO2 0x03

#define TIMEOUT_SEC 2

PanTiltHat::PanTiltHat()
        : isSetup(0), idleTimeoutSec(TIMEOUT_SEC), enableServo1(0), enableServo2(0), i2cAddress(0x15), i2cRetries(10),
          i2cRetryTime(static_cast<unsigned int>(0.01)) {
    servoMin[0] = 575;
    servoMin[1] = 575;
    servoMax[0] = 2325;
    servoMax[1] = 2325;
}

void PanTiltHat::setup() {
    if (isSetup) return;

    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize wiringPi" << std::endl;
        exit(1);
    }

    isSetup = 1;
}

void PanTiltHat::setConfig() {
    int config = 0;
    config |= enableServo1;
    config |= enableServo2 << 1;

    i2cWriteByte(REG_CONFIG, config);
}

void PanTiltHat::i2cWriteByte(int reg, int data) const {
    int i2cHandle = wiringPiI2CSetup(i2cAddress);
    if (i2cHandle == -1) {
        std::cerr << "Failed to open I2C device" << std::endl;
        exit(1);
    }

    for (int i = 0; i < i2cRetries; i++) {
        if (wiringPiI2CWriteReg8(i2cHandle, reg, data) != -1) {
            return;
        }
        delay(i2cRetryTime * 1000);
    }

    std::cerr << "Failed to write byte" << std::endl;
    exit(1);
}

void PanTiltHat::i2cWriteWord(int reg, int data) const {
    int i2cHandle = wiringPiI2CSetup(i2cAddress);
    if (i2cHandle == -1) {
        std::cerr << "Failed to open I2C device" << std::endl;
        exit(1);
    }
    for (int i = 0; i < i2cRetries; ++i) {
        if (wiringPiI2CWriteReg16(i2cHandle, reg, data) != -1) {
            return;
        }
        delay(i2cRetryTime * 1000);
    }

    std::cerr << "Failed to write word" << std::endl;
}

int PanTiltHat::i2cReadByte(int reg) const {
    int  i2cHandle = wiringPiI2CSetup(i2cAddress);
    if (i2cHandle == -1) {
        std::cerr << "Failed to open I2C device" << std::endl;
        exit(1);
    }

    for (int i = 0; i < i2cRetries; ++i) {
        int data = wiringPiI2CReadReg8(i2cHandle, reg);
        if (data != -1) {
            return data;
        }
        delay(i2cRetryTime * 1000);
    }

    std::cerr << "Failed to read byte" << std::endl;
    exit(1);
}

int PanTiltHat::i2cReadWord(int reg) const {
    int i2cHandle = wiringPiI2CSetup(i2cAddress);
    if (i2cHandle == -1) {
        std::cerr << "Failed to open I2C device" << std::endl;
        exit(1);
    }

    for (int i = 0; i < i2cRetries; ++i) {
        int data = wiringPiI2CReadReg16(i2cHandle, reg);
        if (data != -1) {
            return data;
        }
        delay(i2cRetryTime * 1000);
    }

    std::cerr << "Failed to read word" << std::endl;
    exit(1);
}

void PanTiltHat::servoRange(int servoIndex, int& usMin, int& usMax) {
    usMin = servoMin[servoIndex];
    usMax = servoMax[servoIndex];
}

void PanTiltHat::checkRange(int value, int valueMin, int valueMax) {
    if (value < valueMin || value > valueMax) {
        std::cerr << "Value " << value << " should be between " << valueMin << " and " << valueMax << std::endl;
        exit(1);
    }
}

int PanTiltHat::servoUsToDegrees(int us, int usMin, int usMax) {
    checkRange(us, usMin, usMax);
    auto servoRange = static_cast<float>(usMax - usMin);
    double angle = (static_cast<float>(us - usMin) / servoRange) * 180.0;
    return static_cast<int>(round(angle)) - 90;
}

int PanTiltHat::servoDegreesToUs(int angle, int usMin, int usMax) {
    checkRange(angle, -90, 90);
    angle += 90;
    auto servoRange = static_cast<float>(usMax - usMin);
    double us = (servoRange / 180.0) * angle;
    return usMin + static_cast<int>(round(us));
}

void PanTiltHat::servoOne(int angle) {
    setup();

    if (!enableServo1) {
        enableServo1 = 1;
        setConfig();
    }

    int usMin, usMax;
    servoRange(0, usMin, usMax);

    int us = servoDegreesToUs(angle, usMin, usMax);
    i2cWriteWord(REG_SERVO1, us);

    if (idleTimeoutSec > 0) {
        delay(idleTimeoutSec * 1000);
        enableServo1 = 0;
        setConfig();
    }
}

void PanTiltHat::servoTwo(int angle) {
    setup();

    if (!enableServo2) {
        enableServo2 = 1;
        setConfig();
    }

    int usMin, usMax;
    servoRange(1, usMin, usMax);

    int us = servoDegreesToUs(angle, usMin, usMax);
    i2cWriteWord(REG_SERVO2, us);

    if (idleTimeoutSec > 0) {
        delay(idleTimeoutSec * 1000);
        enableServo2 = 0;
        setConfig();
    }
}

int PanTiltHat::getServoOne() {
    setup();
    int usMin, usMax;
    servoRange(0, usMin, usMax);
    int us = i2cReadWord(REG_SERVO1);
    return servoUsToDegrees(us, usMin, usMax);
}

int PanTiltHat::getServoTwo() {
    setup();
    int usMin, usMax;
    servoRange(1, usMin, usMax);
    int us = i2cReadWord(REG_SERVO2);
    return servoUsToDegrees(us, usMin, usMax);
}

void PanTiltHat::setServoPulseMin(int servo, int pulse) {
    if (servo < 1 || servo > 2) {
        std::cerr << "Invalid servo number: " << servo << std::endl;
        exit(1);
    }
    servoMin[servo - 1] = pulse;
}

void PanTiltHat::setServoPulseMax(int servo, int pulse) {
    if (servo < 1 || servo > 2) {
        std::cerr << "Invalid servo number: " << servo << std::endl;
        exit(1);
    }
    servoMax[servo - 1] = pulse;
}

void PanTiltHat::pan(int angle) {
    servoOne(angle);
}

void PanTiltHat::tilt(int angle) {
    servoTwo(angle);
}

int PanTiltHat::getPan() {
    return getServoOne();

}

int PanTiltHat::getTilt() {
    return getServoTwo();
}


int main() {
    PanTiltHat panTilt;
    panTilt.setup();

    // Set custom pulse widths for servo one
    panTilt.setServoPulseMin(1, 600);
    panTilt.setServoPulseMax(1, 1000);

    // Set custom pulse widths for servo two
    panTilt.setServoPulseMin(2, 600);
    panTilt.setServoPulseMax(2, 1000);

    // Move servo one to 0 degrees and servo two to 45 degrees
    panTilt.pan(0);
    panTilt.tilt(45);

    // Read positions of both servos
    int pulse1 = panTilt.getPan();
    int pulse2 = panTilt.getTilt();
    std::cout << "Servo one position: " << pulse1 << std::endl;
    std::cout << "Servo two position: " << pulse2 << std::endl;

    return 0;
}

