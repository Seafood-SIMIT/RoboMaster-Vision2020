#将名称保存到DIR_ENERGY_SRCS
AUX_SOURCE_DIRECTORY(src DIR_ENERGY_SRCS)
#MESSAGE( STATUS "source file = ${DIR_ENERGY_SRCS}.")
#所有.h文件
FILE(GLOB headers "include/*.h")
#MESSAGE( STATUS "source file = ${headers}.")
#生成链接库
add_library(energy SHARED "${DIR_ENERGY_SRCS}")