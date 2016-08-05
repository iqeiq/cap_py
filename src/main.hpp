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
    Capturer();
    ~Capturer();

    bool init(std::string title);
    boost::numpy::ndarray getFrame();
    
private:
    std::string title;
    std::vector<boost::uint8_t> frame;
    std::pair<int, int> size;
    HWND hWnd;
    HDC hdc;
    boost::uint8_t* pixel;

    void updateFrame();
};

