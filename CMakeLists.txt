#CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

#项目信息
project(vision)


#-----------------------------------------------------------
# 检查c++编译器标志，设置c++11支持变量
#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#if(COMPILER_SUPPORTS_CXX11)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#else()
#    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#C++版本
add_definitions(-std=c++11)
#add_compile_options(-lpthread)

#加入头文件
#FILE(GLOB headers "Codes/*/include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#set(header2son "${headers}" CACHE INTERNAL "头文件传递")



#添加Debug选项
SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")  

#将名称存入DIR_SRCS中
aux_source_directory(. DIR_SRCS)
#-----------------------------------------------------------
#加入include文件
include_directories(Codes/Camera/include
                    Codes/Can/include
                    Codes/Preprocess/include
                    Codes/Energy/include
                    Codes/Kalman/include
                    Codes/ArmorFinder/include
                    Codes/Tools/include
                    Codes/Number/include
                    .)
#-----------------------------------------------------------
#连接标准库
#link_libraries(stdio string stdlib iostream thread iomanip)
#-----------------------------------------------------------
#连接相机库
link_directories(Codes/Camera/lib)
link_libraries(MvCameraControl)

#-----------------------------------------------------------
#连接thread
find_package (Threads REQUIRED)
#link_libraries(${CMAKE_THREAD_LIBS_INIT})


#-----------------------------------------------------------
#指定生成目标
add_executable(vision main.cpp)


find_package (OpenCV REQUIRED)
link_libraries(opencv_tracking)
link_libraries(opencv_videoio)
include_directories(${OPENCV_INCLUDE_DIRS})
#-----------------------------------------------------------
#添加子目录
add_subdirectory(Codes)


#添加链接库
target_link_libraries(vision
                    ${OpenCV_LIBS}
                    ${CMAKE_THREAD_LIBS_INIT}
                    camera
                    armor_finder
                    manifold_can
                    kalman
                    number
                    preprocess
                    tools
                    )
#-----------------------------------------------------------


