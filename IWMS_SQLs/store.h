/************************************************
 * Name: store.h
 * Date: 2020-08-20
 * Description: sqlite3基础封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-20
 ************************************************/

#ifndef __STORE_H__
#define __STORE_H__

#include <sqlite3.h>
#include "common_base.h"
#include "common_time.h"
#include "store_models.h"


#define DB_PATH  "/www/IWMS.db"
#define TABLE_PARAM_CONFIG  "CREATE TABLE IF NOT EXISTS param_config (id INTEGER PRIMARY KEY AUTOINCREMENT,  type INT NOT NULL,  min REAL NOT NULL,  max REAL NOT NULL,  alarm INT NOT NULL,  automation INT NOT NULL,  status INT NOT NULL);"
#define TABLE_TEMPERATURE   "CREATE TABLE IF NOT EXISTS temperature  (id INTEGER PRIMARY KEY AUTOINCREMENT,  time TEXT NOT NULL default (datetime('now', 'localtime')),  value REAL NOT NULL,  status INT NOT NULL,  message TEXT NOT NULL);"
#define TABLE_HUMIDITY      "CREATE TABLE IF NOT EXISTS humidity     (id INTEGER PRIMARY KEY AUTOINCREMENT,  time TEXT NOT NULL default (datetime('now', 'localtime')),  value REAL NOT NULL,  status INT NOT NULL,  message TEXT NOT NULL);"
#define TABLE_ILLUMINATION  "CREATE TABLE IF NOT EXISTS illumination (id INTEGER PRIMARY KEY AUTOINCREMENT,  time TEXT NOT NULL default (datetime('now', 'localtime')),  value REAL NOT NULL,  status INT NOT NULL,  message TEXT NOT NULL);"
#define TABLE_DEVICE_STATUS "CREATE TABLE IF NOT EXISTS device_status(id INTEGER PRIMARY KEY AUTOINCREMENT,  name TEXT NOT NULL,  type INT NOT NULL,  warehouse INT NOT NULL,  status INT NOT NULL,  mode INT NOT NULL);"
#define TABLE_STATISTICS    "CREATE TABLE IF NOT EXISTS statistics   (warehouse INT NOT NULL,  date TEXT NOT NULL,  abnormal_temperature INT NOT NULL default 0,  min_temperature REAL NOT NULL default 0,  max_temperature REAL NOT NULL default 0,  abnormal_humidity INT NOT NULL default 0,  min_humidity REAL NOT NULL default 0,  max_humidity REAL NOT NULL default 0,  abnormal_illuminance INT NOT NULL default 0,  min_illuminance REAL NOT NULL default 0,  max_illuminance REAL NOT NULL default 0, PRIMARY KEY (warehouse, date));"


//************************************************
//  公共方法
//************************************************
//  查询回调类型
typedef void (*dataFormatCall) (char **row, void *outdata);
//  初始化数据库
int initSQL(sqlite3 **out);
//  关闭数据库
int closeSQL(void);
//  查询表格数据
int execQuerySql(char *sql, dataFormatCall call, int datasize, void *outdatas);


//************************************************
//  业务方法
//************************************************
//  创建数据表
int createTables(void);
//  添加默认数据(默认参数配置(温度,湿度,光照), 默认设备状态(蜂鸣器,照明灯,风扇))
int addDefaultData(void);

//  查询 温度/湿度/光照 参数配置
int queryParamConfig(ParamType type, ParamConfigModel *out);
//  修改 温度/湿度/光照 参数配置
int modifyParamConfig(ParamConfigModel *model);
//  添加 温度/湿度/光照 参数配置
int addParamConfig(ParamConfigModel *model);

//  读取 温度数据 最近一条
int queryRecentlyTemperature(time_t time, CommonValueModel *out);
//  读取 温度数据 某一天
int querySomedayTemperature(char *date, int outlen, CommonValueModel *out);
//  设置 温度数据
int addRealtimeTemperature(CommonValueModel *model);
//  读取 湿度数据 最近一条
int queryRecentlyHumidity(time_t time, CommonValueModel *out);
//  读取 湿度数据 某一天
int querySomedayHumidity(char *date, int outlen, CommonValueModel *out);
//  设置 湿度数据
int addRealtimeHumidity(CommonValueModel *model);
//  读取 光照数据 最近一条
int queryRecentlyIllumination(time_t time, CommonValueModel *out);
//  读取 光照数据 某一天
int querySomedayIllumination(char *date, int outlen, CommonValueModel *out);
//  设置 光照数据
int addRealtimeIllumination(CommonValueModel *model);

//  读取 设备状态
int queryDeviceStatus(int warehouse, DeviceType type, DeviceStatusModel *out);
//  修改 设备状态
int modifyDeviceStatus(DeviceStatusModel *model);
//  添加 设备状态
int addDeviceStatus(DeviceStatusModel *model);

//  读取 统计信息数据
int queryStatistics(int warehouse, char *date, StatisticsModel *out);
//  修改 统计信息数据
int modifyStatistics(StatisticsModel *model);


//  格式化 参数配置信息
void formatParamConfigCall(char **row, void *outdata);
//  格式化 通用值数据信息
void formatCommonValueCall(char **row, void *outdata);
//  格式化 设备状态信息
void formatDeviceStatusCall(char **row, void *outdata);
//  格式化 统计信息
void formatStatisticsCall(char **row, void *outdata);

#endif //__STORE_H__
