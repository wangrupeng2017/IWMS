/************************************************
 * Name: zigbee_uart.h
 * Date: 2020-08-26
 * Description: 与zigbee通信的方法封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-26
 ************************************************/
#ifndef __ZIGBEE_UART_H__
#define __ZIGBEE_UART_H__

#include "zigbee_protocol.h"

//  zigbee设备节点路径
#define DEV_ZIGBEE  "/dev/ttyUSB1"

//  消息长度
#define LEN_ENV 20
#define LEN_RFID 4 

//  zigbee连接初始化
int zigbee_init();
//  zigbee发送消息
int zigbee_send(unsigned char cmd); 
//  zigbee接收消息
int zigbee_receive(void *out, int len);
//  zigbee接收环境消息
int zigbee_recv_env(getEnvMsg *env);
//  zigbee响应连接成功
int zigbee_ack();

#endif //__ZIGBEE_UART_H__