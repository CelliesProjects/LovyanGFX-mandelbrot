#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>

void drawMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations);
void drawCoordinateAxes(float xmin, float xmax, float ymin, float ymax, float ticks);

LGFX display;

const int32_t BTN_A = GPIO_NUM_39;

void setup()
{
    display.init();
    display.setRotation(1); // Adjust rotation as needed

    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_DARKGRAY);

    // Mandelbrot parameters
    float xmin = -2.5;
    float xmax = 1.0;
    float ymin = -1.5;
    float ymax = 1.5;
    int maxIterations = 100;

    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.5);

    pinMode(BTN_A, INPUT_PULLUP);

    while (digitalRead(BTN_A))
        delay(10);

    xmin = -1.5;
    xmax = 0.75;
    ymin = 0.0;
    ymax = 1.93;

    display.fillScreen(TFT_BLACK);
    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.5);


    while (digitalRead(BTN_A))
        delay(10);

    xmin = -0.5;
    xmax = 0.5;
    ymin = 0.5;
    ymax = 1.36;

    display.fillScreen(TFT_BLACK);
    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.25);


    while (digitalRead(BTN_A))
        delay(10);

    xmin = -0.25;
    xmax = 0.15;
    ymin = 0.75;
    ymax = 1.09;

    display.fillScreen(TFT_BLACK);
    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.25);    
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

void drawCoordinateAxes(float xmin, float xmax, float ymin, float ymax, float ticks)
{
    int width = display.width();
    int height = display.height();

    // Calculate scale factors for the Cartesian to display coordinate transformation
    float scaleX = (xmax - xmin) / width;
    float scaleY = (ymax - ymin) / height;

    int realAxisY = (0 - ymin) / scaleY;
    display.drawLine(0, realAxisY, width, realAxisY, TFT_DARKGRAY); // Draw horizontal axis (real axis)

    int imaginaryAxisX = (0 - xmin) / scaleX;
    display.drawLine(imaginaryAxisX, 0, imaginaryAxisX, height, TFT_DARKGRAY); // Draw vertical axis (imaginary axis)

    // Draw tick marks on the real axis
    for (float x = xmin; x <= xmax; x += ticks)
    {
        int tickX = (x - xmin) / scaleX;
        display.drawLine(tickX, realAxisY - 5, tickX, realAxisY + 5, TFT_DARKGRAY);

        if (x != 0) // Avoid overlapping the origin
            display.drawFloat(x, 2, tickX - 15, realAxisY + 10, &DejaVu9);
    }

    // Draw tick marks on the imaginary axis
    for (float y = ymin; y <= ymax; y += ticks)
    {
        int tickY = (y - ymin) / scaleY;
        display.drawLine(imaginaryAxisX - 5, tickY, imaginaryAxisX + 5, tickY, TFT_DARKGRAY);

        if (y != 0) // Avoid overlapping the origin
            display.drawFloat(y, 2, imaginaryAxisX + 10, tickY - 5, &DejaVu9);
    }
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
