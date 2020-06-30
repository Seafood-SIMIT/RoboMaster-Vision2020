#include "kalmanfilter.h"

using namespace cv;
using namespace std;
/**
 * @name Kalman::predict
 * @author seafood
 * @par Point input
 * @func    kalman预测函数
 * @return void
 * */
Point2d Kalman::predict(Point input)
{
	measurement=Mat::zeros(measureNum, 1, CV_32F); 
	prediction=KF->predict();
	predict_pt= Point(prediction.at<float>(0),prediction.at<float>(1) );   //预测值(x',y')
	//3.update measurement
	measurement.at<float>(0) = (float)input.x;
	measurement.at<float>(1) = (float)input.y;		
	//4.update
	//更新Error函数
	KF->correct(measurement);
	//cout<<"point:"<<input.x<<","<<input.y<<endl;
	//cout<<"predit_point:"<<predict_pt.x<<","<<predict_pt.y<<endl;
	//返回预测点
	return Point(predict_pt.x,predict_pt.y);
}