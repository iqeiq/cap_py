import logging
import cv2
import capy


def main():
    logger = logging.getLogger(__name__)
    logger.addHandler(logging.StreamHandler())
    logger.setLevel(logging.DEBUG)
    logger.info("test")
    window_name = capy.hello()
    logger.info(window_name)
    capturer = capy.factory()
    if not capturer.init("capy - Microsoft Visual Studio", logger):
        logger.error("init error.")
        return
    cv2.namedWindow(window_name)
    while True:
        img = capturer.getFrame()
        cv2.imshow(window_name, img)
        cv2.waitKey(100)


if __name__ == '__main__':
    main()