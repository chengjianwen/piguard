  /*****************************************************************************
  * 文件名     : piguard.cpp
  * 作者       : pi-classroom.com
  * 版本       : 0.1
  * 说明       : 通过P3-P7控制电机驱动版，以及一个两路继电器。实现门锁控制和LED屏的控制
  *                LU1000
  *                  P3    <-------->   K1
  *                  P4    <-------->   K2
  *                  P5    <-------->
  *                  P6    <-------->   IN1
  *                  P7    <-------->   IN2
  *              程序需要通过pilock或者piunlock的命令来执行
  *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usb2gpio.h"
#include "usb_device.h"

#define		P0	(1 << 0)	//
#define		P1	(1 << 1)	//
#define		P2	(1 << 2)	// K1
#define		P3	(1 << 3)	// K2
#define		P4	(1 << 4)	//
#define		P5	(1 << 5)	// motor-0
#define		P6	(1 << 6)	// motor-1
#define		P7	(1 << 7)	//

int main(int argc, char **argv)
{
    int  handle;
    int  lock = -1;	// lock/unlock
    int  on = -1;	// on/off
    int  index;		// index of on/off

    char *p = argv[0] + strlen(argv[0]);
    while (*(p-1) != '/' && p != argv[0])
        p--;

    if (strcmp(p, "pilock") == 0)
        lock = 1;
    else if (strcmp(p, "piunlock") == 0)
        lock = 0;
    else {
      if (strcmp(p, "pion") == 0)
        on = 1;
      else if (strcmp(p, "pioff") == 0)
        on = 0;

      if (argc != 2)
      {
        fprintf (stderr, "Usage %s <index>\n", argv[0]);
        return -1;
      }
      index = atoi(argv[1]);
      if (index != 1 &&  index != 2)
      {
        fprintf (stderr, "index should be 1 or 2\n");
        return -1;
      }
    }

    if (lock < 0 && on < 0){
        fprintf(stderr, "couldn't resolv the command!\n");
        return -1;
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
                         P0 | P1 | P2 | P3 | P4 | P5 | P6 | P7,	// Pin Mask, 0-2bit
                         0			              	// 0: 无上拉和下拉电阻
                         				        // 1: 使能上拉电阻
                         				        // 2: 使能下拉电阻
                         );
    if (ret != GPIO_SUCCESS){
        printf("error on GPIO_SetOutput(): %d!\n", ret);
        return -6;
    }

    if (lock >= 0) {
      // P6输出高电平，P7输出低电平为正转，反之为反转
      ret = GPIO_Write(handle,		// 设备句柄
                       P5 | P6,		// Pin Mask
                       lock ? P5 : P6);	// Pin Value
      if (ret != GPIO_SUCCESS){
          fprintf(stderr, "error on GPIO_Write(): %d!\n", ret);
          return -1;
      }

      // 等待500毫秒
     // usleep(500000);
      sleep(1);

      // 停止电机
      ret = GPIO_Write(handle,		// 设备句柄
                     P5 | P6,		// Pin Mask
                     0); 		// Pin Value
      if (ret != GPIO_SUCCESS){
          fprintf(stderr, "error on GPIO_Write(): %d!\n", ret);
          return -1;
      }
    } else if (on == 0) {
      if (index == 1)
        ret = GPIO_Write(handle,		// 设备句柄
                         P2,
                         0);
      else if (index == 2)
        ret = GPIO_Write(handle,		// 设备句柄
                         P3,
                         0);
      if (ret != GPIO_SUCCESS) {
        fprintf(stderr, "error on GPIO_Write(): %d!\n", ret);
        return -1;
      }
    } else if (on == 1) {
      if (index == 1)
        ret = GPIO_Write(handle,		// 设备句柄
                         P2,
                         P2);
      else if (index == 2)
        ret = GPIO_Write(handle,		// 设备句柄
                         P3,
                         P3);
      if (ret != GPIO_SUCCESS) {
        fprintf(stderr, "error on GPIO_Write(): %d!\n", ret);
        return -1;
      }
    }
    //关闭设备
    ret = USB_CloseDevice(DevHandle[0]);
    if(!ret){
        printf("error on USB_CloseDevice()!\n");
        return -8;
    }
    return 0;
}

