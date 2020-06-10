# coding=utf-8
import cv2 as cv

if __name__ == '__main__':

    # 开启ip摄像头
    video = "http://admin:admin@192.168.50.167:8081/"
    capture = cv.VideoCapture(video)

    num = 0
    while True:
        ret, img = capture.read()
        if not ret:
            break
        cv.imshow("camera", img)

        # 按键处理，注意，焦点应当在摄像头窗口，不是在终端命令行窗口
        key = cv.waitKey(10)

        if key == 27:
            # esc键退出
            print("esc break...")
            break
