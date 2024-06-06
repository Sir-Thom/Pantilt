#include <iostream>
#include <pantilt.h>// Include the header file

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
