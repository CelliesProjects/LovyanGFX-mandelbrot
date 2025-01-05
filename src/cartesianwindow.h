#ifndef CARTESIANWINDOW_H
#define CARTESIANWINDOW_H

#define LGFX_M5STACK
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>  // クラス"LGFX"を用意します

extern LGFX display; 

class CartesianWindow {
private:
    LGFX& _display;
    float _originX, _originY;
    float _scaleX, _scaleY;

public:
    CartesianWindow(LGFX& display);
    void setWindow(float x, float y, float width, float height);
    void getPixelCoordinates(float x, float y, int16_t &px, int16_t &py) const;
    void drawCartesianPixel(float x, float y, uint16_t color);
};

#endif
