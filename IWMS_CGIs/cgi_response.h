/************************************************
 * Name: cgi_response.h
 * Date: 2020-08-22
 * Description: cgi响应结果方法
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-22
 ************************************************/

//  响应结果 实时参数配置(温度/湿度/光照)
int sendRealtimeParam(void *res, int reslen);
//  响应结果 设备状态(蜂鸣器/照明灯/风扇)
int sendDeviceStatus(void *res, int reslen);
//  响应结果 NULL
int sendNULL(void *res, int reslen);
//  响应结果 实时走势数据
int sendRealtimeTrend(void *res, int reslen);
//  响应结果 异常统计信息
int sendAbnormalStatistics(void *res, int reslen);
//  响应结果 历史走势数据
int sendHistoryTrend(void *res, int reslen);

