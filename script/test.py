import cv2

import capy


def main():
    window_name = capy.hello()
    capturer = capy.factory("capy - Microsoft Visual Studio")
    cv2.namedWindow(window_name)
    while True:
        img = capturer.getFrame()
        cv2.imshow(window_name, img)
        cv2.waitKey(100)


if __name__ == '__main__':
    main()