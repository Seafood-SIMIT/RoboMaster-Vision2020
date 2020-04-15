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

extern bool show_armor_box;                 //显示目标装甲板
extern bool show_armor_boxes;               //显示装甲板s
extern bool show_light_blobs;               //显示灯条
extern bool show_origin;                    //显示源图像
extern bool choose_source;                //选择输入源,默认摄像机
//extern bool save_video;                     //储存图像
//extern bool wait_can;                      //等待总线
//extern bool save_labelled_boxes;            //储存打标签的装甲板
extern bool show_process;                   
extern bool show_energy;
extern bool save_mark;
extern bool show_info;
extern bool run_by_frame;


void processOptions(int argc, char **argv);

#endif /* _OPTIONS_H_ */
