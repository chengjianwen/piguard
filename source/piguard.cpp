  /*****************************************************************************
  * 文件名     : piguard.cpp
  * 作者       : pi-classroom.com
  * 版本       : 0.1
  * 说明       : 通过P0-P2控制L298N的ENA、IN1和IN2，实现门锁的开锁和关锁
  *                LU1000              L298N
  *                  P0    <-------->   ENA
  *                  P1    <-------->   IN1
  *                  P2    <-------->   IN2
  *              程序需要通过pilock或者piunlock的命令来执行
  *****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "usb2gpio.h"
#include "usb_device.h"

int main(int argc, const char* argv[])
{
    int  handle;
    int  lock = -1;

    char *p;
    p = (char *)argv[0] + strlen(argv[0]);
    while (*(p-1) != '/' && p != argv[0])
        p--;

    if (strcmp(p, "pilock") == 0)
        lock = 1;
    else if (strcmp(p, "piunlock") == 0)
        lock = 0;

    if (lock < 0){
        printf("Neigher lock nor unlock!\n");
        return 0;
    }

    //扫描查找设备
    int DevHandle[10];
    int ret = USB_ScanDevice(DevHandle);
    if(ret < 0){
        printf("error on USB_ScanDevice()!\n");
        return -1;
    }
    if(ret == 0){
        printf("No device found!\n");
        return -2;
    }
    if(ret > 1){
        printf("Too many devices found!\n");
        return -3;
    }

    handle = DevHandle[0];
    //打开设备
    ret = USB_OpenDevice(handle);
    if(!ret){
        printf("error on USB_OpenDevice()!\n");
        return -4;
    }

    //设置GPIO电平（需要适配器支持）
    ret = DEV_SetPowerLevel(handle, POWER_LEVEL_3V3);
    if(!ret){
        printf("error on DEV_SetPowerLevel()!\n");
        return -5;
    }

    //设置输出模式――没上下拉电阻
    ret = GPIO_SetOutput(handle, 
                         0xF,		// Pin Mask, 0-15bit
                         0		// 0: 无上拉和下拉电阻
                         		// 1: 使能上拉电阻
                         		// 2: 使能下拉电阻
                         );
    if (ret != GPIO_SUCCESS){
        printf("error on GPIO_SetOutput(): %d!\n", ret);
        return -6;
    }

    // 控制电机正转
    // P0输出高电平，使能L298N的A通道
    // P1输出低电平，P2输出高电平，正转
    // 输出信号为: 1 << 0 | 0 << 1 | 1 << 2
    ret = GPIO_Write(handle,			// 设备句柄
                     0x7,			// Pin Mask，0-2bit
                     lock ? 1<< 0 | 0 << 1 | 1 << 2 : 1 << 0 | 1 << 1 | 0 << 2	// Pin Value，lock时0-2bit分别为1-0-1，unlock时0-2bit位分别为1-1-0
                     );
    if (ret != GPIO_SUCCESS){
        printf("error on GPIO_Write(): %d!\n", ret);
        return -7;
    }
    usleep(500000);	// 延时0.5秒，即500000微秒

    // 电机停止
    // P0输出低电平，使能L298N的A通道
    // 输出信号为: 0 << 0
    ret = GPIO_Write(handle,	// 设备句柄
                     0x1,	// Pin Mask，0bit
                     0 << 0 	// Pin Value，0bit为0
                     );
    if (ret != GPIO_SUCCESS){
        printf("error on GPIO_Write(): %d!\n", ret);
        return -7;
    }

    //关闭设备
    ret = USB_CloseDevice(DevHandle[0]);
    if(!ret){
        printf("error on USB_CloseDevice()!\n");
        return -8;
    }
    return 0;
}

