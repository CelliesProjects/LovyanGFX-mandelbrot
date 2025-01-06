#include "cartesianwindow.h"

// Constructor
CartesianWindow::CartesianWindow(LGFX &display) : _display(display) {
    _xMin = _yMin = 0;
    _xRange = _yRange = 1;
}

void CartesianWindow::setWindow(float xMin, float yMin, float xRange, float yRange) {
    _xMin = xMin;
    _yMin = yMin;
    _xRange = xRange;
    _yRange = yRange;

    _displayWidth = _display.width();
    _displayHeight = _display.height();
}

void CartesianWindow::drawCartesianPixel(float x, float y, uint16_t color) {
    int16_t px, py;
    getPixelCoordinates(x, y, px, py);

    if (px >= 0 && px < _displayWidth && py >= 0 && py < _displayHeight) {
        _display.drawPixel(px, py, color);
    }
}

void CartesianWindow::getPixelCoordinates(float x, float y, int16_t &px, int16_t &py) {
    px = (int16_t)(_displayWidth * (x - _xMin) / _xRange);
    py = (int16_t)(_displayHeight * (y - _yMin) / _yRange);

    // Flip Y-axis for Cartesian origin at bottom-left
    py = _displayHeight - py;
}

LGFX &CartesianWindow::getDisplay() {
    return _display;
}
