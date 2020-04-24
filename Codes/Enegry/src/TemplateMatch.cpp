
#include"energy.h"
using namespace cv;
using namespace std;

//模板匹配
double TemplateMatch(cv::Mat image, cv::Mat tepl, cv::Point &point, int method)
{
    int result_cols =  image.cols - tepl.cols + 1;
    int result_rows = image.rows - tepl.rows + 1;
//    cout <<result_cols<<" "<<result_rows<<endl;
    cv::Mat result = cv::Mat( result_cols, result_rows, CV_32FC1 );
    cv::matchTemplate( image, tepl, result, method );

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    switch(method)
    {
    case CV_TM_SQDIFF:
    case CV_TM_SQDIFF_NORMED:
        point = minLoc;
        return minVal;

    default:
        point = maxLoc;
        return maxVal;

    }
}
