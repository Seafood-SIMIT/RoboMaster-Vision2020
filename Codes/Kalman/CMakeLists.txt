#将名称保存到DIR_KALMAN_SRCS
AUX_SOURCE_DIRECTORY(src DIR_KALMAN_SRCS)
#MESSAGE( STATUS "source file = ${DIR_KALMAN_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(kalman SHARED "${DIR_KALMAN_SRCS}")