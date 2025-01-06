#include "cartesianwindow.h"

void CartesianWindow::setWindow(float x, float y, float width, float height)
{
    _originX = x;
    _originY = y;
    _scaleX = width / _display.width();
    _scaleY = height / _display.height();
}

void CartesianWindow::getPixelCoordinates(float x, float y, int16_t &px, int16_t &py) const
{
    px = static_cast<int16_t>((x - _originX) / _scaleX);
    py = static_cast<int16_t>((y - _originY) / _scaleY);

    // Flip Y-axis to match Cartesian convention
    py = _display.height() - py - 1;
}

void CartesianWindow::drawCartesianPixel(float x, float y, uint16_t color)
{
    int16_t px, py;
    getPixelCoordinates(x, y, px, py);

    if (px >= 0 && px < _display.width() && py >= 0 && py < _display.height())
    {
        _display.drawPixel(px, py, color);
    }
}
