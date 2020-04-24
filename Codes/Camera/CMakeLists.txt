#查找当前目录下的所有源文件
#将名称保存到DIR_CAMERA_SRCS
aux_source_directory(src DIR_CAMERA_SRCS)

#生成链接库
add_library(MathFunctions $(DIR_CAMERA_SRCS))