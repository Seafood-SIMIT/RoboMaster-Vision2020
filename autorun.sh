#!/bin/bash
#开机自启动脚本
#Author Seafood
#Generation 1
#建立脚本，使其开机自配置ip地址，can总线地址、波特率啥
echo dji | sudo -S ifconfig can0 down
sudo ip link set can0 type can bitrate 1000000
ip -details link show can0
sudo ifconfig can0 up

#开机设置
sudo ./jetson_clocks.sh

#路径设置
export LD_LIBRARY_PATH=/usr/local/lib
