#include "cartesianwindow.h"

// Set up the Cartesian coordinate system
void CartesianWindow::setWindow(float x, float y, float width, float height)
{
    _originX = x;                         // Cartesian origin X
    _originY = y;                         // Cartesian origin Y
    _scaleX = width / _display.width();   // Scale factor for X
    _scaleY = height / _display.height(); // Scale factor for Y
}

// Convert Cartesian coordinates to pixel coordinates
void CartesianWindow::getPixelCoordinates(float x, float y, int16_t &px, int16_t &py) const
{
    px = (x - _originX) / _scaleX; // Map Cartesian X to pixel X
    py = (y - _originY) / _scaleY; // Map Cartesian Y to pixel Y

    // Flip Y-axis to match Cartesian convention
    py = _display.height() - py - 1;
}

// Draw a pixel in Cartesian coordinates
void CartesianWindow::drawCartesianPixel(float x, float y, uint16_t color)
{
    int16_t px, py;
    getPixelCoordinates(x, y, px, py);

    // Draw the pixel if within screen bounds
    if (px >= 0 && px < _display.width() && py >= 0 && py < _display.height())
    {
        _display.drawPixel(px, py, color);
    }
}

