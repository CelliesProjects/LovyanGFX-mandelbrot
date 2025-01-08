#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>

// Function declarations
void drawMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations);
void drawCoordinateAxes(float xmin, float xmax, float ymin, float ymax, float ticks);
void drawZoomedMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations, float ticks);
void showZoomedArea(float x, float y, float zoomfactor, float xmin, float xmax, float ymin, float ymax);
void setCoordinates(float x, float y, float zoomFactor, float &xmin, float &xmax, float &ymin, float &ymax);

// Global objects
LGFX display;

const int32_t BTN_A = GPIO_NUM_39;

float xmin = -2.5;
float xmax = 1.0;
float ymin = -1.5;
float ymax = 1.5;

void waitForButton(int32_t button)
{
    while (digitalRead(button))
        delay(10);
}

void setup()
{
    display.init();
    display.setRotation(1); // Adjust rotation as needed

    pinMode(BTN_A, INPUT_PULLUP);

    drawMandelbrot(xmin, xmax, ymin, ymax, 100);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 1);
    showZoomedArea(-0.1, 0.95, 5, xmin, xmax, ymin, ymax);
    waitForButton(BTN_A);

    display.fillScreen(TFT_BLACK);
    setCoordinates(-0.1, 0.95, 5, xmin, xmax, ymin, ymax);
    drawMandelbrot(xmin, xmax, ymin, ymax, 100);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.1);
    showZoomedArea(-0.1, 0.92, 8, xmin, xmax, ymin, ymax);
    waitForButton(BTN_A);

    display.fillScreen(TFT_BLACK);
    setCoordinates(-0.1, 0.95, 8, xmin, xmax, ymin, ymax);
    drawMandelbrot(xmin, xmax, ymin, ymax, 100);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.1);
    showZoomedArea(-0.102, 0.924, 12, xmin, xmax, ymin, ymax);
    waitForButton(BTN_A);

    display.fillScreen(TFT_BLACK);
    setCoordinates(-0.102, 0.924, 12, xmin, xmax, ymin, ymax);
    drawMandelbrot(xmin, xmax, ymin, ymax, 200);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, 0.1);
    showZoomedArea(-0.102, 0.924, 16, xmin, xmax, ymin, ymax);
    waitForButton(BTN_A);

}

void loop()
{
    // Nothing to update in the loop for static display
}

void setCoordinates(float x, float y, float zoomFactor, float &xmin, float &xmax, float &ymin, float &ymax)
{
    // Calculate the current width and height of the complex plane
    float currentWidth = xmax - xmin;
    float currentHeight = ymax - ymin;

    // Calculate the new width and height after applying the zoom factor
    float zoomedWidth = currentWidth / zoomFactor;
    float zoomedHeight = currentHeight / zoomFactor;

    // Ensure the aspect ratio is preserved
    if (display.width() > display.height())
    {
        zoomedWidth = zoomedHeight * display.width() / display.height();
    }
    else
    {
        zoomedHeight = zoomedWidth * display.height() / display.width();
    }

    // Set the new bounds centered around (x, y)
    xmin = x - zoomedWidth / 2;
    xmax = x + zoomedWidth / 2;
    ymin = y - zoomedHeight / 2;
    ymax = y + zoomedHeight / 2;
}

void showZoomedArea(float x, float y, float zoomfactor, float xmin, float xmax, float ymin, float ymax)
{
    int width = display.width();
    int height = display.height();

    // Calculate the size of the zoomed area in the complex plane
    float zoomedWidth = (xmax - xmin) / zoomfactor;
    float zoomedHeight = (ymax - ymin) / zoomfactor;

    // Ensure the aspect ratio is preserved
    if (width > height)
    {
        zoomedWidth = zoomedHeight * width / height;
    }
    else
    {
        zoomedHeight = zoomedWidth * height / width;
    }

    // Calculate the bounds of the zoomed area in the complex plane
    float zoomXmin = x - zoomedWidth / 2;
    float zoomXmax = x + zoomedWidth / 2;
    float zoomYmin = y - zoomedHeight / 2;
    float zoomYmax = y + zoomedHeight / 2;

    // Map the zoomed area's bounds to display coordinates
    float scaleX = (xmax - xmin) / width;
    float scaleY = (ymax - ymin) / height;

    int rectX1 = (zoomXmin - xmin) / scaleX;
    int rectX2 = (zoomXmax - xmin) / scaleX;
    int rectY1 = height - (zoomYmin - ymin) / scaleY;
    int rectY2 = height - (zoomYmax - ymin) / scaleY;

    // Draw the rectangle
    display.drawRect(rectX1, rectY2, rectX2 - rectX1, rectY1 - rectY2, TFT_RED);
}

// Utility function to map Mandelbrot parameters, draw, and wait for input
void drawZoomedMandelbrot(float xmin, float xmax, float ymin, float ymax, int maxIterations, float ticks)
{
    display.fillScreen(TFT_BLACK); // Clear the screen
    drawMandelbrot(xmin, xmax, ymin, ymax, maxIterations);
    drawCoordinateAxes(xmin, xmax, ymin, ymax, ticks);

    // Wait for button press
    while (digitalRead(BTN_A))
        delay(10);
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

    int realAxisY = height - (0 - ymin) / scaleY;
    display.drawLine(0, realAxisY, width, realAxisY, TFT_DARKGRAY); // Draw horizontal axis (real axis)

    int imaginaryAxisX = (0 - xmin) / scaleX;
    display.drawLine(imaginaryAxisX, 0, imaginaryAxisX, height, TFT_DARKGRAY); // Draw vertical axis (imaginary axis)

    // Draw tick marks on the real axis
    for (float x = xmin; x <= xmax; x += ticks)
    {
        int tickX = (x - xmin) / scaleX;
        display.drawLine(tickX, realAxisY - 5, tickX, realAxisY + 5, TFT_DARKGRAY);

        if (x != 0) // Avoid overlapping the origin
            display.drawFloat(x, 2, tickX - 15, realAxisY - 15, &DejaVu9);
    }

    // Draw tick marks on the imaginary axis
    for (float y = ymin; y <= ymax; y += ticks)
    {
        int tickY = height - (y - ymin) / scaleY;
        display.drawLine(imaginaryAxisX - 5, tickY, imaginaryAxisX + 5, tickY, TFT_DARKGRAY);

        if (y != 0) // Avoid overlapping the origin
            display.drawString(String(y) + "i", imaginaryAxisX + 10, tickY - 5, &DejaVu9);
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
            float y0 = ymax - py * scaleY; // Invert the y-axis mapping
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
