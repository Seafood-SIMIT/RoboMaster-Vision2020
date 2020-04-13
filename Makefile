#Makefile for canny
#--------------------------变量定义-----------------------------------------------
#目标名
TARGET := vision

LIBS := MvCameraControl

CAMERA_DIR=./Codes/Camera
SOURCE_CAMERA_DIR=$(CAMERA_DIR)/src

NUMBER_DIR=./Codes/Number
SOURCE_NUMBER_DIR=$(NUMBER_DIR)/src

ARMORFINDER_DIR=./Codes/ArmorFinder
SOURCE_ARMORFINDER_DIR=$(ARMORFINDER_DIR)/src

ENERGY_DIR=./Codes/Enegry
SOURCE_ENERGY_DIR=$(ENERGY_DIR)/src

PREPOCESS_DIR=./Codes/Preprocess
SOURCE_PREPOCESS_DIR=$(PREPOCESS_DIR)/src
#头文件
INC := ./ ./Codes/ArmorFinder/include ./Codes/Camera/include ./Codes/Preprocess/include ./Codes/Enegry/include ./Codes/Number/include

#动态库或静态库文件的相对路径
LIBSPATH := ./Codes/Camera/lib

VPATH=$(SOURCE_CAMERA_DIR):$(SOURCE_ARMORFINDER_DIR):$(SOURCE_ENERGY_DIR):$(SOURCE_PREPOCESS_DIR):$(SOURCE_NUMBER_DIR)
#定义变量pkg表示opencv的包
PKG = `pkg-config opencv --libs --cflags`


#删除指令变量参数
RM = rm -rf

#编译器为g++
CC = g++ -std=c++11
#编译时的参数配置
CFLAGS = -g -o 
#编译时所依赖的通用c++源程序
#源程序文件
SOURCE :=$(wildcard $(SOURCE_CAMERA_DIR)/*.cpp $(SOURCE_ARMORFINDER_DIR)/*.cpp $(SOURCE_ENERGY_DIR)/*.cpp $(SOURCE_PREPOCESS_DIR)/*.cpp $(SOURCE_NUMBER_DIR)/*.cpp main.cpp) 
SOURCE_WITHOUT_DIR :=$(notdir $(SOURCE))
OBJS :=$(patsubst %.cpp,%.o,$(SOURCE_WITHOUT_DIR))

#--------------------------命令操作----------------------------------------------
#$^ 代表所有依赖文件
#$@ 代表目标文件
#$< 代表依赖第一个文件
all:$(TARGET)


$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $@ $^ $(addprefix -L,$(LIBSPATH)) $(addprefix -l,$(LIBS)) $(PKG) $(addprefix -I,$(INC))

%.o:%.cpp
	$(CC) -c $^ $(CFLAGS) $@  $(addprefix -I,$(INC)) $(PKG)

#---------------------------伪指令---------------------------------------------
#clean操作，删除生成的文件和文件夹
#如运行过需执行sudo操作
.PHONY:clean
clean:
	$(RM) $(TARGET) $(OBJS) 

#伪指令add-library
#将动态库安装到系统路径中。
.PHONY:add-libraries
add-libraries:
	sudo cp ./Codes/Camera/lib/* /usr/local/lib

.PHONY:show
show:
	@echo $(SOURCE_WITHOUT_DIR)
	@echo $(OBJS)
