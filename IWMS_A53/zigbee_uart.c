/************************************************
 * Name: zigbee_uart.c
 * Date: 2020-08-26
 * Description: 与zigbee通信的方法封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-26
 ************************************************/

#include "zigbee_uart.h"

//  zigbee文件描述符
static int zigbee_fd = -1;

//  zigbee连接初始化
int zigbee_init()
{
    //  打开zigbee串口设备节点
    zigbee_fd = open(DEV_ZIGBEE, O_RDWR);
    if (zigbee_fd < 0)
    {
        perror ("open ttyUSB");
		puts("!!!Please insert your Zigbee device!");
        return -1;
    }

    //  ↓↓↓ 串口初始化 ↓↓↓
	struct termios options;
	tcgetattr(zigbee_fd, &options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB; 
	options.c_iflag |= IGNPAR;
	options.c_iflag &= ~(ICRNL | IXON);
	options.c_oflag = 0;
	options.c_lflag = 0;

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	tcsetattr(zigbee_fd,TCSANOW,&options);
    //  ↑↑↑ 串口初始化 ↑↑↑

    return 0;
}

//  zigbee发送消息
int zigbee_send(unsigned char cmd)
{
    //  在命令上组合上仓库号
    cmd |= (STORE_NO << 6);
    //  发送命令
    int ret = write(zigbee_fd, &cmd, 1);
    if (ret < 0)
    {
        perror ("zigbee_send::write zigbee_fd");
        return -1;
    }
    return 0;
}

//  zigbee接收消息
int zigbee_receive(void *out, int len)
{
    int ret = 0;
    int recvlen = 0;
    do {
        ret = read(zigbee_fd, out, len-recvlen);
        if (ret < 0)
        {
            perror ("zigbee_receive::read zigbee_fd");
            return -1;
        }
        recvlen += ret;
    } while(recvlen < len);

    return 0;
}

//  zigbee接收环境消息
int zigbee_recv_env(getEnvMsg *env)
{
    int ret    = 0;
    char check = 0;
    char buff[20] = "";

    //  接收消息
    ret = zigbee_receive(&check, 1);
    if (ret < 0) return -1;

    //  判断消息类型
    if (check == 'c')
        zigbee_send(MSG_CONNECT_SUCCESS);
    else if (check == 's')
    {
        //  接收消息, 判断消息类型
        //  st:r 表示货物信息
        //  st:e 表示环境信息
        ret = zigbee_receive(&check, 1);
        if ((ret<0) || (check!='t')) return -1;
        ret = zigbee_receive(&check, 1);
        if ((ret<0) || (check!=':')) return -1;
        ret = zigbee_receive(&check, 1);
        if (ret < 0) return -1;

        //  非环境信息, 从接收缓冲区清除掉
        if (check == 'r')
        {
            zigbee_receive(buff, LEN_RFID);
            return -1;
        }
        //  环境信息, 通过参数进行传出
        else if (check == 'e')
        {
            zigbee_receive(buff, LEN_ENV);
        }
    }

    // 传出环境数据
    memcpy(env, buff, LEN_ENV);
    return 0;
}

//  zigbee响应连接成功
int zigbee_ack()
{
    //  响应连接成功消息
    zigbee_send(MSG_CONNECT_SUCCESS);
    return 0;
}

