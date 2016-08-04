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


Capturer* getInstance(std::string title) {
    return new Capturer(title);
}

boost::numpy::ndarray Capturer::getFrame() {
    updateFrame();
    namespace bp = boost::python;
    namespace np = boost::numpy;
    return np::from_data(
        frame.data(),
        np::dtype::get_builtin<boost::uint8_t>(),
        bp::make_tuple(size.second, size.first, 4),
        bp::make_tuple(4 * size.first, 4, 1),
        bp::object());
}

Capturer::Capturer(std::string title): title(title) { init(); }
Capturer::~Capturer() {}

void Capturer::init() {
    HWND desktop = GetDesktopWindow();
    HWND child = NULL;
    while(true) {
        child = FindWindowEx(desktop, child, NULL, NULL);
        if(child == NULL) break;
        char str[512];
        GetWindowText(child, str, 512);
        std::cout << "window: " << str << std::endl;
        if(title == str) { hWnd = child; break; }
    }
    if(hWnd == NULL) return;
    std::cout << title << " found!" << std::endl;

    RECT rc;
    GetWindowRect(hWnd, &rc);
    size.first = rc.right - rc.left;
    size.second = rc.bottom - rc.top;
    pos.first = rc.left;
    pos.second = rc.top;
    frame.resize(size.first * size.second * 4);

    BITMAPINFO bmpInfo;

    //DIBの情報を設定する
    bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth=size.first;
    bmpInfo.bmiHeader.biHeight=size.second;
    bmpInfo.bmiHeader.biPlanes=1;
    bmpInfo.bmiHeader.biBitCount=32;
    bmpInfo.bmiHeader.biCompression=BI_RGB;

    //DIBSection作成
    HDC hdcScreen = GetDC(NULL);
    hdc = CreateCompatibleDC(hdcScreen);
    hdc2 = CreateCompatibleDC(hdcScreen);
    hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
    LPDWORD lpPixel2;
    HBITMAP hBitmap2 = CreateDIBSection(hdc2, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel2, NULL, 0);
    SelectObject(hdc, hBitmap);
    SelectObject(hdc2, hBitmap2);
}

void Capturer::updateFrame() {
    PrintWindow(hWnd, hdc2, PW_CLIENTONLY);
    StretchBlt(hdc, 0, size.second, size.first, -size.second, hdc2, 0, 0, size.first, size.second, SRCCOPY);
    std::copy_n((boost::uint8_t*)lpPixel, frame.size(), frame.data());
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
    class_<Capturer>("Capturer", init<std::string>())
        .def("getFrame", &Capturer::getFrame);

}
