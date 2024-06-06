#ifndef PANTILTHAT_H
#define PANTILTHAT_H

class PanTiltHat {
public:
    PanTiltHat();
    void setup();
    void idleTimeout(int value);
    void servoEnable(int index, int state);
    void servoPulseMax(int index, int value);
    void servoPulseMin(int index, int value);
    int getPan();
    int getTilt();
    void pan(int angle);
    void tilt(int angle);
    void setServoPulseMin(int servo, int pulse);  // Add this declaration
    void setServoPulseMax(int servo, int pulse);  // Add this declaration

private:
    void setConfig();
    void i2cWriteByte(int reg, int data) const;
    void i2cWriteWord(int reg, int data) const;
    int i2cReadByte(int reg) const;
    int i2cReadWord(int reg) const;
    void servoOne(int angle);
    void servoTwo(int angle);
    int getServoOne();
    int getServoTwo();
    static void checkRange(int value, int valueMin, int valueMax);
    static int servoUsToDegrees(int us, int usMin, int usMax);
    static int servoDegreesToUs(int angle, int usMin, int usMax);
    void servoRange(int servoIndex, int& usMin, int& usMax);

    int isSetup;
    int idleTimeoutSec;
    int enableServo1;
    int enableServo2;
    int servoMin[2]{};
    int servoMax[2]{};
    int i2cAddress;
    int i2cRetries;
    unsigned int i2cRetryTime;
};

#endif /* PANTILTHAT_H */
