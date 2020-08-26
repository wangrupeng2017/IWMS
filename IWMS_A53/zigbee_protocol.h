#ifndef __ZIGBEE_PROTOCOL_H__
#define __ZIGBEE_PROTOCOL_H__

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/input.h>


#define QUEUE_MSG_LEN		32
#define GOODS_NUM			10
#define STORAGE_NUM 		5

#define MSG_M0				4L
#define MSG_CONNECT_SUCCESS 'Y'
#define MSG_M0_FAN_OFF		0x00
#define MSG_M0_FAN_ON1		0x01
#define MSG_M0_FAN_ON2		0x02
#define MSG_M0_FAN_ON3		0x03

#define MSG_M0_BEEP_OFF		0x10
#define MSG_M0_BEEP_ON		0x11
#define MSG_M0_BEEP_AU_OFF	0x12
#define MSG_M0_BEEP_AU_ON	0x13

#define MSG_M0_LED_OFF		0x20
#define MSG_M0_LED_ON		0x21

#define MSG_M0_SEG_ON		0x30
#define MSG_M0_SEG_OFF		0x3f


typedef struct storage_goods_info
{
	unsigned char goods_type;
	unsigned int goods_count;
} storage_goods_info;

typedef struct storage_info
{
	unsigned char storage_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char buzzer_status;
	unsigned char fan_status;
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	char samplingTime[20];
	float temperature;
	float temperatureMIN;
	float temperatureMAX;
	float humidity;
	float humidityMIN;
	float humidityMAX;
	float illumination;
	float illuminationMIN;
	float illuminationMAX;
	float battery;
	float adc;
	float adcMIN;
	struct storage_goods_info goods_info[GOODS_NUM];
} storage_info;

typedef struct env_info_clien_addr
{
	struct storage_info storage_no[STORAGE_NUM];	
} env_info_clien_addr;

typedef struct msg
{
	long type;
	long msgtype;
	unsigned char text[QUEUE_MSG_LEN];
} msg;

typedef struct getEnvMsg
{
	unsigned char sto_no;
	unsigned char tem[2];
	unsigned char hum[2];
	unsigned char x;
	unsigned char y;
	unsigned char z;
	unsigned int ill;
	unsigned int battery;
	unsigned int adc;
} getEnvMsg;

typedef struct getGoodsMsg
{
	unsigned char sto_no;
	unsigned char io;
	unsigned char goodsno;
	unsigned char goodsnum;
} getGoodsMsg;




#endif //__ZIGBEE_PROTOCOL_H__