#define LGFX_M5STACK
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>  // クラス"LGFX"を用意します
#include "cartesianwindow.h"


// Create an instance of the LovyanGFX display
LGFX display; 

// Create the CartesianWindow and link it to the display
CartesianWindow cartesianWindow(display);

void setup() {
    display.init();                     // Initialize the M5Stack Fire display
    display.setRotation(1);             // Set display rotation (landscape)

    cartesianWindow.setWindow(-2.0, -1.5, 3.0, 3.0); // Set a 3x3 Cartesian space

    // Annotate the Cartesian space
    int16_t px, py;
    cartesianWindow.getPixelCoordinates(-1.0, 0.5, px, py);
    display.setTextColor(TFT_WHITE);
    display.drawString("Point (-1.0, 0.5)", px, py);

    // Draw something in the Cartesian space
    cartesianWindow.drawCartesianPixel(-1.0, 0.5, TFT_WHITE);
}

void loop() {
    // Additional drawing or interaction logic
}
