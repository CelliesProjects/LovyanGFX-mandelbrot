#include <LovyanGFX.hpp>
#include "cartesianwindow.h"

// Create an instance of the LovyanGFX display
LGFX display;

// Create the CartesianWindow and link it to the display
CartesianWindow cartesianWindow(display);

float mapf(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void drawMandelbrot(CartesianWindow &cartesianWindow, float xMin, float xMax, float yMin, float yMax, int maxIterations) {
    LGFX &display = cartesianWindow.getDisplay();

    for (int i = 0; i < display.width(); i++) {
        for (int j = 0; j < display.height(); j++) {
            float x0 = mapf(i, 0, display.width(), xMin, xMax);
            float y0 = mapf(j, 0, display.height(), yMin, yMax);

            float x = 0.0, y = 0.0;
            int iteration = 0;

            while (x * x + y * y <= 4.0 && iteration < maxIterations) {
                float xTemp = x * x - y * y + x0;
                y = 2.0 * x * y + y0;
                x = xTemp;
                iteration++;
            }

            uint16_t color;
            if (iteration == maxIterations) {
                color = TFT_YELLOW; // Points inside the Mandelbrot set are black
            } else {
                uint8_t r = (iteration * 9) % 256;
                uint8_t g = (iteration * 5) % 256;
                uint8_t b = (iteration * 3) % 256;
                color = display.color565(r, g, b);
            }

            cartesianWindow.drawCartesianPixel(x0, y0, color);
        }
    }
}

void setup() {
    display.init();
    display.setRotation(1);

    float xMin = -1.5, xMax = 1.0; 
    float yMin = -1.0, yMax = 1.0;
    int maxIterations = 450;

    cartesianWindow.setWindow(xMin, yMin, xMax - xMin, yMax - yMin);
    drawMandelbrot(cartesianWindow, xMin, xMax, yMin, yMax, maxIterations);
}

void loop() {
    // Additional logic here
}
