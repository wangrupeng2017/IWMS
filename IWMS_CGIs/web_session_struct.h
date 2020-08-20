/************************************************
 * Name: web_session_struct.c 
 * Date: 2020-08-20
 * Description: web通信协议结构体
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-20
 ************************************************/

#ifndef __WEB_SESSION_STRUCT_H__
#define __WEB_SESSION_STRUCT_H__

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

// 错误状态码
typedef enum ErrorStatusCode {
	Success            = 0,
	ErrorRequestMethod = -1,
	ErrorReadContent   = -2,
	ErrorJsonParse     = -3,
} ErrorStatusCode;

// 仓库id请求结构体
typedef struct WarehouseId {
	uint warehouse_id;  //仓库id
} WarehouseId;
// 仓库id历史请求结构体
typedef struct WarehouseHistory {
	uint warehouse_id;  //仓库id
	char date[20];      //日期
} WarehouseHistory;
// 仓库 蜂鸣器/LED/风扇 配置结构体
typedef struct WarehouseDevice {
	uint  warehouse_id;	//仓库id
	int  buzzer;   //蜂鸣器状态 (0:关, 1:开)
	int  light;	   //照明灯状态 (0:关, 1:开)
	int  fan;	   //风扇状态 (0:关，1:1档，2:2档，3:3档)
} WarehouseDevice;
// 仓库 温度/湿度/光度 配置结构体
typedef struct WarehouseConfig {
	uint  warehouse_id;	//仓库id
	float min;          //最小值
	float max;          //最大值
	int   alarm;        //监控报警
	int   automation;   //自动控制
} WarehouseConfig;

// 实时属性配置结构体
typedef struct WebRealtimeParam {
	float value;        //实时值
	float min;          //最小值
	float max;          //最大值
	int   alarm;        //监控报警
	int   automation;   //自动控制
	int   status;       //异常状态
} WebRealtimeParam;
// 实时器件状态结构体
typedef struct WebDeviceStatus {
	int  buzzer;   //蜂鸣器状态 (0:关, 1:开)
	int  light;	   //照明灯状态 (0:关, 1:开)
	int  fan;	   //风扇状态 (0:关，1:1档，2:2档，3:3档)
} WebDeviceStatus;
// 实时走势数据结构体
typedef struct WebRealtimeTrend {
	char  time[20];     //时间
	float temperature;  //温度
	float humidity;     //湿度
	float illuminance;  //照明
} WebRealtimeTrend;
// 异常信息统计结构体
typedef struct WebAbnormalStatistics {
	int abnormalTemperature;  //温度异常次数
	float minTemperature;     //最小温度值
	float maxTemperature;     //最大温度值
	int abnormalHumidity;     //湿度异常次数
	float minHumidity;        //最小湿度值
	float maxHumidity;        //最大湿度值
	int abnormalIlluminance;  //光照异常次数
	float minIlluminance;     //最小光照值
	float maxIlluminance;	  //最大光照值
} WebAbnormalStatistics;
	

#endif //__WEB_SESSION_STRUCT_H__
