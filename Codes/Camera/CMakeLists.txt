
#将名称保存到DIR_CAMERA_SRCS
AUX_SOURCE_DIRECTORY(src DIR_CAMERA_SRCS)
#MESSAGE( STATUS "source file = ${DIR_CAMERA_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
add_definitions(-std=c++11)
#生成链接库
add_library(camera SHARED "${DIR_CAMERA_SRCS}")