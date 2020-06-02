
#将名称保存到DIR_ARMORFINDER_SRCS
AUX_SOURCE_DIRECTORY(src DIR_ARMORFINDER_SRCS)
#MESSAGE( STATUS "source file = ${DIR_ARMORFINDER_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(armor_finder SHARED "${DIR_ARMORFINDER_SRCS}")