#CMake最低版本要求
cmake_minimum_required(VERSION 2.8)

#项目信息
project(vision)

#查找当前目录下的所有源文件
#将名称存入DIR_SRCS中
aux_source_directory(. DIR_SRCS)

#加入一个配置头文件，用于处理CMAKE对源码的设置
configure_file(
"${PROJECT_SOURCE_DIR}/config.h.in"
"${PROJECT_SOURCE_DIR}/config.h"
)

#是否使用自己的CAMERA库
option(USE_CAMERA "Use Provided Camera implementation" ON)

#是否加入Camera库
if(USE_CAMERA)
include_directories("${PROJECT_SOURCE_DIR}/Camera")
#添加子目录
add_subdirectory(Camera)
set(EXTRA_LIBS $(EXTRA_LIBS)CameraInit)
endif(USE_CAMERA)


#指定生成目标
add_executable(vision main.cpp)

#添加链接库
target_link_libraries(vision CameraInit)