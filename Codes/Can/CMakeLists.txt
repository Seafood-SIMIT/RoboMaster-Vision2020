
#将名称保存到DIR_CAN_SRCS
AUX_SOURCE_DIRECTORY(src DIR_CAN_SRCS)
#MESSAGE( STATUS "source file = ${DIR_CAN_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(manifold_can SHARED "${DIR_CAN_SRCS}")