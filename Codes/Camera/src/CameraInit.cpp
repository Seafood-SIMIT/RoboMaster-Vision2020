/*-----------------------------主函数--------------------
*   文件名：CameraInit.cpp
*   作者：  孙霖
*   功能：  摄像头打开设置关闭
------------------------------------------------------*/
#include "CameraInit.h"
//-------------------------函数区------------------------------
/**
 * @author      Seafood
 * @name        PrintDeviceInfo
 * @return      bool
 * @function    打印摄像头信息
 * @para        pstMVDevInfo
 * */
bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    //查看是否有devInfolist
    if (NULL == pstMVDevInfo)
    {
        printf("%s\n" , "The Pointer of pstMVDevInfoList is NULL!");
        return false;
    }
    //网络摄像头
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
		// 打印当前相机ip和用户自定义名字
		// print current ip and user defined name
        printf("%s %x\n" , "nCurrentIp:" , pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp);                 
        printf("%s %s\n\n" , "chUserDefinedName:" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);   
    }
    //USB摄像头
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName:%s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
    }
    //其他
    else
    {
        printf("Not support.\n");
    }
    return true;
}
void paraInit()
{
    //一帧数据大小
    nBuffSize = MAX_IMAGE_DATA_SIZE;
    nRet  = -1 ;                    //相机状态代码
    handle  = NULL ;              //相机用参数  
    pFrameBuf = NULL;            //相机位置
    if(mcu_data.env_light > 0 && mcu_data.env_light < 20){
        fExposureTime=1000;
    }
    else if(mcu_data.env_light >= 20 && mcu_data.env_light < 40){
        fExposureTime=3000;
    }
    else if(mcu_data.env_light >= 40 && mcu_data.env_light < 60){
        fExposureTime=5000;
    }
    else if(mcu_data.env_light >= 60 && mcu_data.env_light < 80){
        fExposureTime=7000;
    }
    else{
        fExposureTime=EXPO_CLASS_5;
    }
}
/**
 * @author      Seafood
 * @name        cameraInit
 * @return      int
 * @function    打开摄像头
 * @para        None
 * */
int cameraInit()
{

    paraInit();
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // 枚举设备
	// enum device
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
    if (MV_OK != nRet)
    {
        printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
        return -1;
    }
    unsigned int nIndex = 0;
    if (stDeviceList.nDeviceNum > 0)
    {
        for (int i = 0; i < stDeviceList.nDeviceNum; i++)
        {
            //printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
            if (NULL == pDeviceInfo)
            {
                break;
            } 
            //PrintDeviceInfo(pDeviceInfo);            
        }  
    } 
    else
    {
        printf("Find No Devices!\n");
        return -1;
    }

    //scanf("%d", &nIndex);

    // 选择设备并创建句柄
	// select device and create handle
    nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        printf("MV_CC_CreateHandle fail! nRet [%x]\n", nRet);
        return -1;
    }

    // 打开设备
	// open device
    nRet = MV_CC_OpenDevice(handle);
    if (MV_OK != nRet)
    {
        printf("MV_CC_OpenDevice fail! nRet [%x]\n", nRet);
        return -1;
    }
    nRet = MV_CC_SetFloatValue(handle, "ExposureTime", fExposureTime);
    // 开始取流
	// start grab image
    nRet = MV_CC_StartGrabbing(handle);
    if (MV_OK != nRet)
    {
        printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
        return -1;
    }

    return 1;

}
/**
 * @author      Seafood
 * @name        cameraExit
 * @return      int
 * @function    摄像头退出
 * @para        None
 * */
int cameraExit()
{
    // 停止取流
	// end grab image
    nRet = MV_CC_StopGrabbing(handle);
    if (MV_OK != nRet)
    {
        printf("MV_CC_StopGrabbing fail! nRet [%x]\n", nRet);
        return -1;
    }

    // 关闭设备
	// close device
    nRet = MV_CC_CloseDevice(handle);
    if (MV_OK != nRet)
    {
        printf("MV_CC_CloseDevice fail! nRet [%x]\n", nRet);
        return -1;
    }

    // 销毁句柄
	// destroy handle
    nRet = MV_CC_DestroyHandle(handle);
    if (MV_OK != nRet)
    {
        printf("MV_CC_DestroyHandle fail! nRet [%x]\n", nRet);
        return -1;
    }
	
	printf("exit\n");
    return 1;
}