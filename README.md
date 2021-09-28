# PiGuard

一个通过LU1000、L298N控制门锁开关的程序

## 使用方法

pilock: 上锁
piunlock: 开锁
pion <1 | 2>: 开继电器
piooff <1 | 2>: 关继电器

## 编译和安装

需要安装libusb-1.0.0-dev，安装命令为：

sudo apt install libusb-1.0-0-dev

将相应平台下的libUSB2XXX.so（参考LU1000官方SDK）拷贝到系统目录/usr/lib下

然后进入linux/piguard目录下用make进行编译

执行
ls -s piguard pilock
ls -s piguard piunlock
ls -s piguard pion
ls -s piguard pioff

生成pilock, piunlock, pion和pioff程序

## 其它

复制stm.rules到/etc/udev/rules.d/中

sudo cp lu1000.rules /etc/udev/rules.d/

