#将名称保存到DIR_TOOLS_SRCS
AUX_SOURCE_DIRECTORY(src DIR_TOOLS_SRCS)
#MESSAGE( STATUS "source file = ${DIR_TOOLS_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(tools SHARED "${DIR_TOOLS_SRCS}")