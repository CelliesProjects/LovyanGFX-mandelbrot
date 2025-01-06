#include <LGFX_AUTODETECT.hpp> 
#include <LovyanGFX.hpp>

void drawMandelbrot();

// Create an instance of the display object based on your specific platform define
LGFX display;

void setup() {
    // Initialize the display
    display.init();
    display.setRotation(1); // Adjust rotation as needed

    // Set background color to black
    display.fillScreen(TFT_BLACK);

    // Draw the Mandelbrot set
    drawMandelbrot();
}

void loop() {
    // Nothing to update in the loop for static display
}

void drawMandelbrot() {
    int width = display.width();
    int height = display.height();

    // Mandelbrot parameters (adjust as needed)
    float xmin = -2.5;
    float xmax = 1.0;
    float ymin = -1.5;
    float ymax = 1.5;
    int maxIterations = 100;

    // Calculate scale factors for the Cartesian to display coordinate transformation
    float scaleX = (xmax - xmin) / width;
    float scaleY = (ymax - ymin) / height;

    // Loop through each pixel on the display
    for (int py = 0; py < height; py++) {
        for (int px = 0; px < width; px++) {
            // Map the pixel coordinates to the complex plane
            float x0 = xmin + px * scaleX;
            float y0 = ymin + py * scaleY;
            float x = 0.0;
            float y = 0.0;
            int iteration = 0;

            // Mandelbrot iteration
            while (x * x + y * y <= 4 && iteration < maxIterations) {
                float xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iteration++;
            }

            // Color the pixel based on the number of iterations
            uint16_t color = (iteration == maxIterations) ? TFT_BLACK : map(iteration, 0, maxIterations * 7, 0, 255);
            display.drawPixel(px, py, color);
        }
    }
}
