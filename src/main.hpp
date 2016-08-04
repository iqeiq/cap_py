#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <windows.h>


#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/numpy.hpp>

#if _MSC_VER
FILE _iob[] ={*stdin, *stdout, *stderr};
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
#endif


class Capturer {
public:
    boost::numpy::ndarray getFrame();
    explicit Capturer(std::string title);
    ~Capturer();
    
private:
    std::string title;
    std::vector<boost::uint8_t> frame;
    std::pair<int, int> size;
    std::pair<int, int> pos;
    HWND hWnd;
    HDC hdc;
    HDC hdc2;
    HBITMAP hBitmap;
    LPDWORD lpPixel;

    void init();
    void updateFrame();
};

