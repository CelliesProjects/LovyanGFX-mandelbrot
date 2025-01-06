#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>

void drawMandelbrot();
void drawCoordinateAxes();

// Create an instance of the display object based on your specific platform define
LGFX display;

void setup()
{
    // Initialize the display
    display.init();
    display.setRotation(1); // Adjust rotation as needed

    // Set background color to black
    display.fillScreen(TFT_BLACK);

    drawCoordinateAxes();

    // Draw the Mandelbrot set
    drawMandelbrot();

    drawCoordinateAxes();

}

void loop()
{
    // Nothing to update in the loop for static display
}

uint16_t getColorFromIteration(int iteration, int maxIterations)
{
    // Normalize the iteration count to a 0-255 range
    int colorValue = map(iteration, 0, maxIterations, 0, 255);

    // Create a color by cycling through hues (full RGB spectrum)
    uint8_t red = (colorValue * 2) % 255;   // Cycling red
    uint8_t green = (colorValue * 5) % 255; // Cycling green
    uint8_t blue = (colorValue * 7) % 255;  // Cycling blue

    // Combine the RGB values into a 16-bit RGB565 color
    return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
}

void drawCoordinateAxes() {
    int width = display.width();
    int height = display.height();

    // Mandelbrot parameters (adjust as needed)
    float xmin = -2.5;
    float xmax = 1.0;
    float ymin = -1.5;
    float ymax = 1.5;

    // Calculate scale factors for the Cartesian to display coordinate transformation
    float scaleX = (xmax - xmin) / width;
    float scaleY = (ymax - ymin) / height;

    // Draw the X axis (real axis)
    int realAxisY = height / 2;
    display.drawLine(0, realAxisY, width, realAxisY, TFT_WHITE);  // Draw horizontal axis (real axis)

    // Draw the Y axis (imaginary axis)
    int imaginaryAxisX = width / 2;
    display.drawLine(imaginaryAxisX, 0, imaginaryAxisX, height, TFT_WHITE);  // Draw vertical axis (imaginary axis)

    // Labels for 1, -1, i, and -i
    // Real axis labels
    display.setCursor(imaginaryAxisX + 5, realAxisY - 15);  // Position label for 1
    display.print("1");
    display.setCursor(imaginaryAxisX - 25, realAxisY - 15);  // Position label for -1
    display.print("-1");

    // Imaginary axis labels
    display.setCursor(imaginaryAxisX + 5, 5);  // Position label for i
    display.print("i");
    display.setCursor(imaginaryAxisX + 5, height - 15);  // Position label for -i
    display.print("-i");
}

void drawMandelbrot()
{
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
    for (int py = 0; py < height; py++)
    {
        for (int px = 0; px < width; px++)
        {
            // Map the pixel coordinates to the complex plane
            float x0 = xmin + px * scaleX;
            float y0 = ymin + py * scaleY;
            float x = 0.0;
            float y = 0.0;
            int iteration = 0;

            // Mandelbrot iteration
            while (x * x + y * y <= 4 && iteration < maxIterations)
            {
                float xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iteration++;
            }

            // Only draw the pixel if the point is outside the Mandelbrot set
            if (iteration != maxIterations) {
                uint16_t color = getColorFromIteration(iteration, maxIterations);
                display.drawPixel(px, py, color);
            }
        }
    }
}
