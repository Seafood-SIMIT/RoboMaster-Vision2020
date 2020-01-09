
#include "energy.h"

using namespace cv;
using std::cout;
using std::endl;
using std::vector;

//----------------------------------------------------------------------------------------------------------------------
// 此函数用于显示图像中所有装甲板
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showArmors(std::string windows_name, const cv::Mat &src) {
    if (src.empty())return;
    
    static Mat image2show;
    static Mat image2show2;//最外面的轮廓
    static Mat image2show3;//整个能量机关最外面的轮廓

    static Mat rot_image[5];
    static Mat result[5]; 
    if (src.type() == CV_8UC1) // 黑白图像
    {
        cvtColor(src, image2show, COLOR_GRAY2RGB); 
        //imshow("test1", image2show);
        //waitKey(0);
        cvtColor(src, image2show2, COLOR_GRAY2RGB); 
        cvtColor(src, image2show3, COLOR_GRAY2RGB); 
    } else if (src.type() == CV_8UC3) //RGB 彩色
    {
        image2show = src.clone();
        //imshow("test2", image2show);
        //waitKey(0);
    }
    for (const auto &armor : armors) {
        Point2f vertices[4];      //定义矩形的4个顶点
        armor.points(vertices);   //计算矩形的4个顶点
        for (int i = 0; i < 4; i++)
            line(image2show, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
    }
    imshow(windows_name, image2show);
    waitKey(0);

    /*for (const auto &armor_external : armors_external) {
        int i=0;
        i=i++;
        Point2f vertices2[4];      //定义矩形的4个顶点
        armor_external.points(vertices2);   //计算矩形的4个顶点
        for (int i = 0; i < 4; i++)
            line(image2show2, vertices2[i], vertices2[(i + 1) % 4], Scalar(0, 0, 255), 2);

    Point2f center = armor_external.center;//外接矩形中心点坐标
	Mat rot_mat = getRotationMatrix2D(center, armor_external.angle, 1.0);//求旋转矩阵
	Size dst_sz(image2show2.size());
	warpAffine(image2show2, rot_image, rot_mat, dst_sz);//原图像旋转
	Mat result[i] = rot_image(Rect(center.x - (armor_external.size.width / 2), center.y - (armor_external.size.height/2), armor_external.size.width, armor_external.size.height));//提取ROI
     }
     */
    /*WYL可用
     Point2f vertices2[4];      //定义矩形的4个顶点
        armors_external[3].points(vertices2);   //计算矩形的4个顶点
        for (int i = 0; i < 4; i++)
        line(image2show2, vertices2[i], vertices2[(i + 1) % 4], Scalar(0, 0, 255), 2);
    Point2f center = armors_external[3].center;//外接矩形中心点坐标
	Mat rot_mat = getRotationMatrix2D(center, armors_external[3].angle, 1.0);//求旋转矩阵
	Size dst_sz(image2show2.size());
	warpAffine(image2show2, rot_image, rot_mat, dst_sz);//原图像旋转
	result = rot_image(Rect(center.x - (armors_external[3].size.width / 2), center.y - (armors_external[3].size.height/2), armors_external[3].size.width, armors_external[3].size.height));//提取ROI
    imshow("出来吧", result);
    waitKey(0);
    */
   //sunlin添加
    Point2f vertices2[5][4];     //定义矩形的4个顶点
    Point2f vertices3[4];        //定义整个能量机关的外部四边形顶点
    Point2f center[5];          //5个中心店坐标
    Point2f MostLeftPoint;      //整个能量机关的最外面的点
    Point2f MostRightPoint;
    Point2f MostUpPoint;
    Point2f MostDownPoint;

    Mat rot_mat[5];             //五个旋转图像
    Size dst_sz(image2show2.size());    //图像尺寸
    for(int i = 0;i < 5;i++)
    {
        armors_external[i].points(vertices2[i]);   //计算矩形的4个顶点
    }
    for (int i = 0; i < 5; i++) //5个矩形
    {
        for(int j = 0; j < 4;j++)      //四个角
        {
            line(image2show2, vertices2[i][j], vertices2[i][(j + 1) % 4], Scalar(0, 0, 255), 2);    //划线
        }
        
        center[i] = armors_external[i].center;//外接矩形中心点坐标
        rot_mat[i] = getRotationMatrix2D(center[i], armors_external[i].angle, 1.0);//求旋转矩阵
	    warpAffine(image2show2, rot_image[i], rot_mat[i], dst_sz);//原图像旋转
        
        result[i] = rot_image[i](Rect(center[i].x - (armors_external[i].size.width / 2), center[i].y - (armors_external[i].size.height/2), armors_external[i].size.width, armors_external[i].size.height));//提取ROI
        imshow("出来吧", result[i]);
        waitKey(0);

    }

        MostLeftPoint=vertices2[0][0];
        MostRightPoint=vertices2[0][0];
        MostUpPoint=vertices2[0][0];
        MostDownPoint=vertices2[0][0];
for (int i=0;i<5;i++)                                   //找出能量机关的最外四个点
{
    for (int j=0;j<4;j++)
    {
    if (vertices2[i][j].x<=MostLeftPoint.x)
        MostLeftPoint=vertices2[i][j];
    if (vertices2[i][j].x>MostRightPoint.x)
        MostRightPoint=vertices2[i][j];
    if (vertices2[i][j].y<=MostDownPoint.y)
        MostDownPoint=vertices2[i][j];
    if (vertices2[i][j].y>MostUpPoint.y)
        MostUpPoint=vertices2[i][j];
    }  
}
    vertices3[0].x=MostLeftPoint.x;
    vertices3[0].y=MostDownPoint.y;
    vertices3[1].x=MostRightPoint.x; 
    vertices3[1].y= MostDownPoint.y; 
    vertices3[2].x=MostRightPoint.x;
    vertices3[2].y=MostUpPoint.y;
    vertices3[3].x=MostLeftPoint.x; 
    vertices3[3].y=MostUpPoint.y;  
    for (int i = 0; i < 4; i++)
    {
            line(image2show3, vertices3[i], vertices3[(i + 1) % 4], Scalar(0, 0, 255), 2);
    }
    imshow("整个能量机关", image2show3);
    waitKey(0); 

    /*imshow("rot_image", rot_image);
    waitKey(0);
    imshow("外围轮廓", image2show2);
    waitKey(0);*/

}




