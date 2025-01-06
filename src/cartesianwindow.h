#ifndef CARTESIANWINDOW_H
#define CARTESIANWINDOW_H

#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> 

class CartesianWindow {
public:
    CartesianWindow(LGFX &display);
    void setWindow(float xMin, float yMin, float xRange, float yRange);
    void drawCartesianPixel(float x, float y, uint16_t color);
    void getPixelCoordinates(float x, float y, int16_t &px, int16_t &py);
    LGFX &getDisplay();

private:
    LGFX &_display;
    float _xMin, _yMin, _xRange, _yRange;
    int16_t _displayWidth, _displayHeight;
};

#endif
