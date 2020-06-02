#将名称保存到DIR_PREPROCESS_SRCS
AUX_SOURCE_DIRECTORY(src DIR_PREPROCESS_SRCS)
#MESSAGE( STATUS "source file = ${DIR_PREPROCESS_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(preprocess SHARED "${DIR_PREPROCESS_SRCS}")