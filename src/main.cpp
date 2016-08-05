#include "main.hpp"

namespace bp = boost::python;
namespace np = boost::numpy;

// memo
class gil_lock {
public:
    gil_lock() { state_ = PyGILState_Ensure(); }
    ~gil_lock() { PyGILState_Release(state_); }
private:
    PyGILState_STATE state_;
};


Capturer* getInstance() {
    return new Capturer();
}

boost::numpy::ndarray Capturer::getFrame() {
    updateFrame();
    namespace bp = boost::python;
    namespace np = boost::numpy;
    return np::from_data(
        frame.data(),
        np::dtype::get_builtin<boost::uint8_t>(),
        bp::make_tuple(size.second, size.first, 4),
        bp::make_tuple(size.first * 4, 4, 1),
        bp::object());
}

Capturer::Capturer() {}
Capturer::~Capturer() {}

bool Capturer::init(std::string title_, boost::python::object logger_) {
    title = title_;
    logger = logger_;
    HWND desktop = GetDesktopWindow();
    HWND child = NULL;
    hWnd = NULL;
    while(true) {
        child = FindWindowEx(desktop, child, NULL, NULL);
        if(child == NULL) break;
        char str[512];
        GetWindowText(child, str, 512);
        if(title == str) { hWnd = child; break; }
    }
    if(hWnd == NULL) return false;
    logger.attr("info")("window \"%s\" found!", title);

    RECT rc;
    GetWindowRect(hWnd, &rc);
    size.first = rc.right - rc.left;
    size.second = rc.bottom - rc.top;
    frame.resize(size.first * size.second * 4);

    logger.attr("info")("screen: (%d, %d)", size.first, size.second);

    BITMAPINFO bmpInfo;

    //DIBの情報を設定する
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = size.first;
    bmpInfo.bmiHeader.biHeight = size.second;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression=BI_RGB;

    //DIBSection作成
    HDC hdcScreen = GetDC(NULL);
    hdc = CreateCompatibleDC(hdcScreen); // -> DeleteDC
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&pixel, NULL, 0);
    SelectObject(hdc, hBitmap);
    ReleaseDC(NULL, hdcScreen);

    return true;
}

void Capturer::updateFrame() {
    PrintWindow(hWnd, hdc, PW_CLIENTONLY);
    
    // pixelは下側から始まる仕様
    // ↓これだと上下反転する
    // std::copy_n((boost::uint8_t*)lpPixel, frame.size(), frame.data());
    for(auto i = 0; i < frame.size(); i++) {
        int h = i / (size.first * 4);
        //int w = (i - h * (size.first * 4)) / 4;
        //int c = i % 4;
        //frame[i] = *(pixel + (size.second-1-h)*size.first*4 + w*4 + c);
        frame[i] = *(pixel + i + (size.second-1-2*h)*size.first*4);
    }
    logger.attr("info")("test");
}


int main(int argc, char **argv) {
    return 0;
}

char const* hello() {
    return "hello, world";
}

BOOST_PYTHON_MODULE(capy) {
    using namespace boost::python;

    Py_Initialize();
    np::initialize();
    
    def("hello", hello);
    def("factory", getInstance, return_value_policy<manage_new_object>());
    class_<Capturer>("Capturer", no_init)
        .def("init", &Capturer::init)
        .def("getFrame", &Capturer::getFrame);

}
