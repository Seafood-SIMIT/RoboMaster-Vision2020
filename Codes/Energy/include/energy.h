#include <iostream>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <chrono>
#include <math.h>
using namespace cv;
using namespace cv::ml;
using namespace std;


//bool is_big;//大符模式为true
//bool is_small;//小符模式为true
   
double getDistance(Point A,Point B);//获取点间距离

double TemplateMatch(cv::Mat image, cv::Mat tepl, cv::Point &point, int method);   
#define ENERGY_RED 1
#define ENERGY_BLUE 0