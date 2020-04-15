//
// Created by sjturm on 19-5-17.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio_c.h>


#include "energy.h"
#include "param_struct_define.h"
#include "ArmorFinder.h"
#include "log.h"

#define RECEIVE_LOG_LEVEL LOG_MSG

using namespace std;
using namespace cv;

extern uint8_t last_state;
/*
//extern ArmorFinder armor_finder;
extern Energy energy;

void saveVideos(const cv::Mat &gimbal_src) {
    if (!gimbal_src.empty()) {
        video_writer.write(gimbal_src);
    } else return;
}

void showOrigin(const cv::Mat &src) {
    if (!src.empty()) {
        imshow("origin", src);
        cv::waitKey(1);
    } else return;
}

void extract(cv::Mat &src) {//图像预处理，将视频切成640×480的大小
    if (src.empty()) return;
    float length = static_cast<float>(src.cols);
    float width = static_cast<float>(src.rows);
    if (length / width > 640.0 / 480.0) {
        length *= 480.0 / width;
        resize(src, src, cv::Size(length, 480));
        src = src(Rect((length - 640) / 2, 0, 640, 480));
    } else {
        width *= 640.0 / length;
        resize(src, src, cv::Size(640, width));
        src = src(Rect(0, (width - 480) / 2, 640, 480));
    }
}

double getPointLength(const cv::Point2f &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}
*/