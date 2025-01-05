#include <LovyanGFX.hpp>
// #include <LGFX_AUTODETECT.hpp>  // クラス"LGFX"を用意します
#include "cartesianwindow.h"

// Create an instance of the LovyanGFX display
LGFX display;

// Create the CartesianWindow and link it to the display
CartesianWindow cartesianWindow(display);

void drawMandelbrot(CartesianWindow &cartesianWindow, float xMin, float xMax, float yMin, float yMax, int maxIterations)
{
    // Get the display reference from the CartesianWindow
    LGFX &display = cartesianWindow.getDisplay();

    // Iterate over every pixel in the display
    for (int i = 0; i < display.width(); i++) {
        for (int j = 0; j < display.height(); j++) {
            // Map pixel coordinates to Cartesian coordinates
            float x0 = xMin + (i * (xMax - xMin) / display.width());
            float y0 = yMin + (j * (yMax - yMin) / display.height());

            float x = 0.0, y = 0.0;
            int iteration = 0;

            // Mandelbrot iteration to calculate escape time
            while (x * x + y * y <= 4.0 && iteration < maxIterations) {
                float xTemp = x * x - y * y + x0;
                y = 2.0 * x * y + y0;
                x = xTemp;
                iteration++;
            }

            // Color the point based on the number of iterations it took to escape
            uint16_t color;

            // Points inside the Mandelbrot set (iteration == maxIterations) remain black
            if (iteration == maxIterations) {
                color = TFT_BLACK;  // Black color for points inside the set
            }
            else {
                // Color based on how fast the point escapes
                int escapeColor = (iteration * 255 / maxIterations); // Scale iteration to color range
                color = display.color565(escapeColor, 0, 255 - escapeColor); // Gradient from blue to red
            }

            // Draw the point using the Cartesian coordinate system
            cartesianWindow.drawCartesianPixel(x0, y0, color);
        }
    }
}

void setup()
{
    display.init();         // Initialize the M5Stack Fire display
    display.setRotation(1); // Set display rotation (landscape)
/*
    cartesianWindow.setWindow(-1.0, -1.0, 2.0, 2.0); // Set a 3x3 Cartesian space

    int16_t px, py;
    cartesianWindow.getPixelCoordinates(0, 0, px, py);
    display.setTextColor(TFT_WHITE);
    display.drawCircle(px, py, 5, display.color565(255, 0, 0));
    display.drawString("Point (0, 0)", px, py);

    // Draw something in the Cartesian space
    for (float x = -1.5; x <= 1.5; x = x + 0.01f)
    {
        cartesianWindow.drawCartesianPixel(x, x*x, display.color565(255, 255, 255));
    }

    delay(1000);
*/
    // Set up the view window for the Mandelbrot set
    float xMin = -1.5, xMax = 1.5, yMin = -1.5, yMax = 1.5;
    int maxIterations = 250;

    // Draw the Mandelbrot set
    cartesianWindow.setWindow(xMin, yMin, xMax - xMin, yMax - yMin); // Define the Cartesian space
    drawMandelbrot(cartesianWindow, xMin, xMax, yMin, yMax, maxIterations); // Draw the fractal
}

void loop()
{
    // Additional drawing or interaction logic
}
