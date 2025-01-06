#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>

void drawMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations);
void drawCoordinateAxes(float xmin, float xmax, float ymin, float ymax);

LGFX display;

void setup()
{
    // Initialize the display
    display.init();
    display.setRotation(1); // Adjust rotation as needed

    // Set background color to black
    display.fillScreen(TFT_BLACK);

    // Mandelbrot parameters
    float xmin = -2.5;
    float xmax = 1.0;
    float ymin = -1.5;
    float ymax = 1.5;

    // Draw coordinate axes
    drawCoordinateAxes(xmin, xmax, ymin, ymax);

    // Draw the Mandelbrot set
    int maxIterations = 100;
    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
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

void drawCoordinateAxes(float xmin, float xmax, float ymin, float ymax)
{
    int width = display.width();
    int height = display.height();

    // Calculate scale factors for the Cartesian to display coordinate transformation
    float scaleX = (xmax - xmin) / width;
    float scaleY = (ymax - ymin) / height;

    // Draw the X axis (real axis)
    int realAxisY = (0 - ymin) / scaleY;                         // Map y=0 to display coordinates
    display.drawLine(0, realAxisY, width, realAxisY, TFT_WHITE); // Draw horizontal axis (real axis)

    // Draw the Y axis (imaginary axis)
    int imaginaryAxisX = (0 - xmin) / scaleX;                               // Map x=0 to display coordinates
    display.drawLine(imaginaryAxisX, 0, imaginaryAxisX, height, TFT_WHITE); // Draw vertical axis (imaginary axis)

    // Draw tick marks on the real axis
    for (float x = xmin; x <= xmax; x += 0.5)
    {
        int tickX = (x - xmin) / scaleX;
        display.drawLine(tickX, realAxisY - 5, tickX, realAxisY + 5, TFT_WHITE);

        if (x != 0) // Avoid overlapping the origin
        {
            display.setCursor(tickX - 10, realAxisY + 10);
            display.print(x, 2); // Print tick value with 2 decimal places
        }
    }

    // Draw tick marks on the imaginary axis
    for (float y = ymin; y <= ymax; y += 0.5)
    {
        int tickY = (y - ymin) / scaleY;
        display.drawLine(imaginaryAxisX - 5, tickY, imaginaryAxisX + 5, tickY, TFT_WHITE);

        if (y != 0) // Avoid overlapping the origin
        {
            display.setCursor(imaginaryAxisX + 10, tickY - 5);
            display.print(y, 2); // Print tick value with 2 decimal places
        }
    }
    /*
        // Labels for 1, -1, i, and -i
        // Real axis labels (adjust position further from the origin)
        display.setCursor(imaginaryAxisX + 50, realAxisY - 15);  // Position label for 1
        display.print("1");
        display.setCursor(imaginaryAxisX - 50, realAxisY - 15);  // Position label for -1
        display.print("-1");

        // Imaginary axis labels (position i and -i close to the axis with a small offset)
        display.setCursor(imaginaryAxisX + 5, realAxisY - 40);  // Position label for i (above the origin)
        display.print("i");
        display.setCursor(imaginaryAxisX + 5, realAxisY + 25);  // Position label for -i (below the origin)
        display.print("-i");
    */
}

void drawMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations)
{
    int width = display.width();
    int height = display.height();

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
            if (iteration != maxIterations)
            {
                uint16_t color = getColorFromIteration(iteration, maxIterations);
                display.drawPixel(px, py, color);
            }
        }
    }
}
