//
// Created by seafood on 2020-09-28
//
#ifndef _STATE_TRACKING_TARGET_H_
#define _STATE_TRACKING_TARGET_H_

#include "kalmanfilter.h"
#include "stateSearchingTarget.h"
#include <opencv2/tracking.hpp>
class StateTrackingTarget{
public:
    bool run(cv::Mat &srcImage,cv::Mat &processImage,ArmorBox &target_box, Kalman *kalman_filter,cv::Ptr<cv::Tracker> tracker,int tracking_cnt);      //运行函数
private: 
    StateSearchingTarget state_searching_target_tracking;  
    //ArmorBox target_box;

};
#endif