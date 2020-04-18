//
// Created by xinyang on 19-3-27.
//

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#ifdef PATH
    #define PROJECT_DIR PATH
#else
    #define PROJECT_DIR ""
#endif
//^b*[^:b#/]+.*$
extern bool show_armor_box;                 //显示目标装甲板
extern bool show_armor_boxes;               //显示装甲板s
extern bool show_light_blobs;               //显示灯条
extern bool show_origin;                    //显示源图像
extern bool choose_source_button;                //选择输入源,默认摄像机
//extern bool save_video;                     //储存图像
//extern bool wait_can;                      //等待总线
//extern bool save_labelled_boxes;            //储存打标签的装甲板
extern bool show_process;                       //显示处理图像
extern bool show_preprocess;                    //显示预处理图像
extern bool show_energy;                        //显示能量机关处理过程
extern bool save_mark;                          //储存标志
extern bool show_info;                          //显示信息
extern bool run_by_frame;                       //以frame运行
extern bool show_details_process;               //显示细节处理
extern bool show_number_ROI_processed;          //显示数字ROI

void processOptions(int argc, char **argv);

#endif /* _OPTIONS_H_ */
