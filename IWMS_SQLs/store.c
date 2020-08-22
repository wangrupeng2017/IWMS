/************************************************
 * Name: store.c 
 * Date: 2020-08-20
 * Description: sqlite3基础封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-20
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "store.h"

// 数据库对象实例
static sqlite3* db_instance = NULL;


//************************************************
//  公共方法
//************************************************
/*
 * function:    initSQL
 * description: 实例化sqlite3对象
 * @param [out] out sqlite3对象实例
 * @return      0:处理成功 !0:处理出错
 */
int initSQL(sqlite3 **out)
{
	// 打开数据库
	int ret = sqlite3_open(DB_PATH, &db_instance);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "打开数据库:", db_instance, return -1);
	puts("打开数据库成功");

	*out = db_instance;
	return 0;
}

/*
 * function:    closeSQL 
 * description: 关闭sqlite对象
 * @return      0:处理成功 !0:处理出错
 */
int closeSQL()
{
	int ret = sqlite3_close(db_instance);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "关闭数据库:", db_instance, return -1);
    puts("关闭数据库成功");

	return 0;
}

/*
 * function:    查询操作的基础封装
 * @param [ in] sql SQL语句
 * @param [ in] call 数据结构格式化回调方法
 * @param [ in] datasize 格式化后的元素数据大小
 * @param [out] outdatas 传出的数据
 * @return      操作结果：>=0:查询结果数，<0:出错
 */
int execQuerySql(char *sql, dataFormatCall call, int datasize, void *outdatas)
{
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;

	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	if (ret!=SQLITE_OK) return -1;

	int i = 0;
	for (i=1; i<=nRow; i++)
	{
		call(result+(nColumn*i), outdatas+(datasize*(i-1)));
	}

	sqlite3_free_table(result);
	return nRow;
}


//************************************************
//  业务方法
//************************************************
//  创建数据表
int createTables(void)
{
	int ret = 0;

    ret = sqlite3_exec(db_instance, TABLE_PARAM_CONFIG,  NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[param_config]:",  db_instance, return -1);
	ret = sqlite3_exec(db_instance, TABLE_TEMPERATURE,   NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[temperature]:",   db_instance, return -1);
	ret = sqlite3_exec(db_instance, TABLE_HUMIDITY,      NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[humidity]:",      db_instance, return -1);
	ret = sqlite3_exec(db_instance, TABLE_ILLUMINATION,  NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[illumination]:",  db_instance, return -1);
	ret = sqlite3_exec(db_instance, TABLE_DEVICE_STATUS, NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[device_status]:", db_instance, return -1);
	ret = sqlite3_exec(db_instance, TABLE_STATISTICS,    NULL, NULL, NULL);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格[statistics]:",    db_instance, return -1);

	puts("创建表格成功");
	return 0;
}

//  添加默认数据(默认参数配置(温度,湿度,光照), 默认设备状态(蜂鸣器,照明灯,风扇))
int addDefaultData(void)
{
/**							 
INSERT INTO param_config(type, min, max, alarm, automation, status) VALUES('%d', '%f', '%f', '%d', '%d', '%d');
**/
	ParamConfigModel param_config[3] = {
		[0] = { 
			.type       = TypeTemperature,
			.min        = 0,
			.max        = 60,
			.alarm      = 0,
			.automation = 0,
			.status     = 0
		},
		[1] = { 
			.type       = TypeHumidity,
			.min        = 0,
			.max        = 90,
			.alarm      = 0,
			.automation = 0,
			.status     = 0
		},
		[2] = { 
			.type       = TypeIllumination,
			.min        = 0,
			.max        = 100,
			.alarm      = 0,
			.automation = 0,
			.status     = 0
		},
	};
/**
INSERT INTO device_status(name, type, warehouse, status, mode) VALUES('%f', '%d', '%d', '%d', '%d');
**/
	DeviceStatusModel device_status[3] = {
		[0] = {
			.name      = "蜂鸣器",
			.type      = TypeBuzzer,
			.warehouse = 1,
			.status    = 0,
			.mode      = TypeAutomatic
		},
		[1] = {
			.name      = "照明灯",
			.type      = TypeLight,
			.warehouse = 1,
			.status    = 0,
			.mode      = TypeAutomatic
		},
		[2] = {
			.name      = "风扇",
			.type      = TypeFan,
			.warehouse = 1,
			.status    = 0,
			.mode      = TypeAutomatic
		}
	};

	int ret = 0;
	//  温度配置
	int rows = queryParamConfig(param_config[0].type, &(param_config[0]));
	TRY_SQLITE_ERROR(rows<0, "查询温度参数配置:", db_instance, return -1);
	if (rows==0) 
	{
		ret = addParamConfig(&(param_config[0]));
		TRY_SQLITE_ERROR(ret<0, "添加默认温度参数配置:", db_instance, return -1);
	}
	//  湿度配置
	rows = queryParamConfig(param_config[1].type, &(param_config[1]));
	TRY_SQLITE_ERROR(rows<0, "查询湿度参数配置:", db_instance, return -1);
	if (rows==0) 
	{
		ret = addParamConfig(&(param_config[1]));
		TRY_SQLITE_ERROR(ret<0, "添加默认湿度参数配置:", db_instance, return -1);
	}
	//  光照配置
	rows = queryParamConfig(param_config[2].type, &(param_config[2]));
	TRY_SQLITE_ERROR(rows<0, "查询光照参数配置:", db_instance, return -1);
	if (rows==0) 
	{
		ret = addParamConfig(&(param_config[2]));
		TRY_SQLITE_ERROR(ret<0, "添加默认光照参数配置:", db_instance, return -1);
	}

	//  蜂鸣器设备状态
	rows = queryDeviceStatus(device_status[0].warehouse, device_status[0].type, &(device_status[0]));
	TRY_SQLITE_ERROR(rows<0, "查询蜂鸣器设备状态:", db_instance, return -1);
	if (rows==0)
	{
		ret = addDeviceStatus(&(device_status[0]));
		TRY_SQLITE_ERROR(ret<0, "添加默认蜂鸣器设备状态:", db_instance, return -1);
	}
	//  照明设备状态
	rows = queryDeviceStatus(device_status[1].warehouse, device_status[1].type, &(device_status[1]));
	TRY_SQLITE_ERROR(rows<0, "查询照明设备状态:", db_instance, return -1);
	if (rows==0)
	{
		ret = addDeviceStatus(&(device_status[1]));
		TRY_SQLITE_ERROR(ret<0, "添加默认照明设备状态:", db_instance, return -1);
	}
	//  风扇设备状态
	rows = queryDeviceStatus(device_status[2].warehouse, device_status[2].type, &(device_status[2]));
	TRY_SQLITE_ERROR(rows<0, "查询风扇设备状态:", db_instance, return -1);
	if (rows==0)
	{
		ret = addDeviceStatus(&(device_status[2]));
		TRY_SQLITE_ERROR(ret<0, "添加默认风扇设备状态:", db_instance, return -1);
	}

	return 0;
}

//  查询 温度/湿度/光照 参数配置
int queryParamConfig(ParamType type, ParamConfigModel *out)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
	char *sql_format = "SELECT id, type, min, max, alarm, automation, status \
						FROM param_config  WHERE type='%d' limit 1;";
	sprintf(sql, sql_format, type);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatParamConfigCall, sizeof(ParamConfigModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询参数配置:", db_instance, return -1);

	return rows;
}
//  修改 温度/湿度/光照 参数配置
int modifyParamConfig(ParamConfigModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "UPDATE param_config \
                       SET min='%f', max='%f', alarm='%d', automation='%d', status='%d' \
                       WHERE type='%d';";                                           
    sprintf(sql, sql_format, model->min, model->max, model->alarm, \
							 model->automation, model->status, model->type);

	// 执行修改SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "修改参数配置:", db_instance, return -1);

	return 0;
}
//  添加 温度/湿度/光照 参数配置
int addParamConfig(ParamConfigModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "INSERT INTO param_config(type, min, max, alarm, automation, status) \
						VALUES('%d', '%f', '%f', '%d', '%d', '%d');";                                           
    sprintf(sql, sql_format, model->type, model->min, model->max, model->alarm, \
							 model->automation, model->status);

	// 执行创建SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建参数配置:", db_instance, return -1);

	return 0;					 
}

//  读取 温度数据 最近一条
int queryRecentlyTemperature(time_t time, CommonValueModel *out)
{
	// 查询给定时间 5分钟内最近一条数据
	char time1_str[20] = "";
	char time2_str[20] = "";
	timestampToTimestr(time-(60*5), time1_str);
	timestampToTimestr(time,        time2_str);

	// 格式化SQL语句
	char sql[256]  = "";
	char *sql_format = "SELECT id, time, value, status, message \
					    FROM temperature  WHERE time>'%s' and time<'%s' \
					    ORDER BY time DESC  limit 1;";
    sprintf(sql, sql_format, time1_str, time2_str);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatCommonValueCall, sizeof(CommonValueModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询温度数据:", db_instance, return -1);

	return rows;
}
//  读取 温度数据 某一天
int querySomedayTemperature(char *date, int outlen, CommonValueModel *out)
{
	//  按照查询次数 分隔时间段查询 
	//  从该天的0点开始每个间隔时间查询1次
	int time_interval = DAY_SECONDS / outlen;
	time_t t = datestrToTimestamp(date);

	int i    = 0;
	int ret  = 0;
	int rows = 0;
	for (i=0; i<outlen; i++)
	{
		// 执行查询
		ret = queryRecentlyTemperature(t+(time_interval*i), (out+i));
		TRY_SQLITE_ERROR(ret<0, "查询某天温度数据:", db_instance, return -1);

		rows += ret;
		// 查询不到也要填写上时间
		timestampToTimestr(t+(time_interval*i), (out+i)->time);
	}

	return rows;
}
//  设置 温度数据
int addRealtimeTemperature(CommonValueModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "INSERT INTO temperature(value, status, message) \
						VALUES('%f', '%d', '%s');";                                           
    sprintf(sql, sql_format, model->value, model->status, model->message);

	// 执行创建SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建温度数据:", db_instance, return -1);

	return 0;
}
//  读取 湿度数据 最近一条
int queryRecentlyHumidity(time_t time, CommonValueModel *out)
{
	// 查询给定时间 5分钟内最近一条数据
	char time1_str[20] = "";
	char time2_str[20] = "";
	timestampToTimestr(time-(60*5), time1_str);
	timestampToTimestr(time,        time2_str);

	// 格式化SQL语句
	char sql[256]  = "";
	char *sql_format = "SELECT id, time, value, status, message \
					    FROM humidity  WHERE time>'%s' and time<'%s' \
					    ORDER BY time DESC  limit 1;";
    sprintf(sql, sql_format, time1_str, time2_str);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatCommonValueCall, sizeof(CommonValueModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询湿度数据:", db_instance, return -1);

	return rows;
}
//  读取 温度数据 某一天
int querySomedayHumidity(char *date, int outlen, CommonValueModel *out)
{
	//  按照查询次数 分隔时间段查询 
	//  从该天的0点开始每个间隔时间查询1次
	int time_interval = DAY_SECONDS / outlen;
	time_t t = datestrToTimestamp(date);

	int i    = 0;
	int ret  = 0;
	int rows = 0;
	for (i=0; i<outlen; i++)
	{
		// 执行查询
		ret = queryRecentlyHumidity(t+(time_interval*i), (out+i));
		TRY_SQLITE_ERROR(ret<0, "查询某天湿度数据:", db_instance, return -1);

		rows += ret;
		// 查询不到也要填写上时间
		timestampToTimestr(t+(time_interval*i), (out+i)->time);
	}

	return rows;
}
//  设置 温度数据
int addRealtimeHumidity(CommonValueModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "INSERT INTO humidity(value, status, message) \
						VALUES('%f', '%d', '%s');";                                           
    sprintf(sql, sql_format, model->value, model->status, model->message);

	// 执行创建SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建湿度数据:", db_instance, return -1);

	return 0;
}
//  读取 光照数据 最近一条
int queryRecentlyIllumination(time_t time, CommonValueModel *out)
{
	// 查询给定时间 5分钟内最近一条数据
	char time1_str[20] = "";
	char time2_str[20] = "";
	timestampToTimestr(time-(60*5), time1_str);
	timestampToTimestr(time,        time2_str);

	// 格式化SQL语句
	char sql[256]  = "";
	char *sql_format = "SELECT id, time, value, status, message \
					    FROM illumination  WHERE time>'%s' and time<'%s' \
					    ORDER BY time DESC  limit 1;";
    sprintf(sql, sql_format, time1_str, time2_str);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatCommonValueCall, sizeof(CommonValueModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询光照数据:", db_instance, return -1);

	return rows;
}
//  读取 光照数据 某一天
int querySomedayIllumination(char *date, int outlen, CommonValueModel *out)
{
	//  按照查询次数 分隔时间段查询 
	//  从该天的0点开始每个间隔时间查询1次
	int time_interval = DAY_SECONDS / outlen;
	time_t t = datestrToTimestamp(date);

	int i    = 0;
	int ret  = 0;
	int rows = 0;
	for (i=0; i<outlen; i++)
	{
		// 执行查询
		ret = queryRecentlyIllumination(t+(time_interval*i), (out+i));
		TRY_SQLITE_ERROR(ret<0, "查询某天光照数据:", db_instance, return -1);

		rows += ret;
		// 查询不到也要填写上时间
		timestampToTimestr(t+(time_interval*i), (out+i)->time);
	}

	return rows;
}
//  设置 光照数据
int addRealtimeIllumination(CommonValueModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "INSERT INTO illumination(value, status, message) \
						VALUES('%f', '%d', '%s');";                                           
    sprintf(sql, sql_format, model->value, model->status, model->message);

	// 执行创建SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建光照数据:", db_instance, return -1);

	return 0;
}


//  读取 设备状态
int queryDeviceStatus(int warehouse, DeviceType type, DeviceStatusModel *out)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
	char *sql_format = "SELECT id, name, type, warehouse, status, mode \
						FROM device_status  WHERE warehouse='%d' AND type='%d' limit 1;";
	sprintf(sql, sql_format, warehouse, type);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatDeviceStatusCall, sizeof(DeviceStatusModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询设备状态:", db_instance, return -1);

	return rows;
}
//  修改 设备状态
int modifyDeviceStatus(DeviceStatusModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "UPDATE device_status \
                       SET status='%d', mode='%d' \
                       WHERE warehouse='%d' AND type='%d';";
    sprintf(sql, sql_format, model->status, model->mode, model->warehouse, model->type);

	// 执行修改SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "修改设备状态:", db_instance, return -1);

	return 0;
}
//  添加 设备状态
int addDeviceStatus(DeviceStatusModel *model)
{
	// 格式化SQL语句
	char sql[256]  = "";                                                               
    char *sql_format = "INSERT INTO device_status(name, type, warehouse, status, mode) \
						VALUES('%f', '%d', '%d', '%d', '%d');";                                           
    sprintf(sql, sql_format, model->name, model->type, model->warehouse, model->status, model->mode);

	// 执行创建SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建设备状态:", db_instance, return -1);

	return 0;	
}


//  读取 统计信息数据
int queryStatistics(int warehouse, char *date, StatisticsModel *out)
{
	// 格式化SQL语句
	char sql[512]  = "";                                                               
	char *sql_format = "SELECT id, warehouse, date, \
						abnormal_temperature, min_temperature, max_temperature, \
						abnormal_humidity, min_humidity, max_humidity, \
						abnormal_illuminance, min_illuminance, max_illuminance \
						FROM statistics  WHERE warehouse='%d' AND date='%s' limit 1;";
	sprintf(sql, sql_format, warehouse, date);

	// 执行查询SQL
	int rows = execQuerySql(sql, formatStatisticsCall, sizeof(StatisticsModel), out);
	TRY_SQLITE_ERROR(rows<0, "查询统计信息数据:", db_instance, return -1);

	return rows;
}
//  修改 统计信息数据
int modifyStatistics(StatisticsModel *model)
{
	// 格式化SQL语句
	char sql[512]  = "";                                                               
	char *sql_format = "REPLACE INTO statistics(id, warehouse, date, \
						abnormal_temperature, min_temperature, max_temperature, \
						abnormal_humidity, min_humidity, max_humidity, \
						abnormal_illuminance, min_illuminance, max_illuminance) \
						VALUES('%d', '%d', '%s', '%d', '%f', '%f', '%d', '%f', '%f', '%d', '%f', '%f');";

	sprintf(sql, sql_format, model->id, model->warehouse, model->date, \
	                        model->abnormal_temperature, model->min_temperature, model->max_temperature, \
							model->abnormal_humidity,    model->min_humidity,    model->max_humidity, \
							model->abnormal_illuminance, model->min_illuminance, model->max_illuminance);

	// 执行 创建或修改 SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);                    
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "修改统计信息数据:", db_instance, return -1);

	return 0;
}


//  格式化 参数配置信息
void formatParamConfigCall(char **row, void *outdata)
{
    ParamConfigModel *info = outdata;

    info->id         = atoi(row[0]);
    info->type       = atoi(row[1]);
    info->min        = atof(row[2]);
    info->max        = atof(row[3]);
    info->alarm      = atoi(row[4]);
    info->automation = atoi(row[5]);
    info->status     = atoi(row[6]);
}                                                   
//  格式化 通用值数据信息
void formatCommonValueCall(char **row, void *outdata)
{
    CommonValueModel *info = outdata;

    info->id       = atoi(row[0]);
    info->value    = atof(row[2]);
    info->status   = atoi(row[3]);
	strcpy(info->time,    row[1]);
	strcpy(info->message, row[4]);
}   
//  格式化 设备状态信息
void formatDeviceStatusCall(char **row, void *outdata)
{
    DeviceStatusModel *info = outdata;

    info->id        = atoi(row[0]);
    info->type      = atof(row[2]);
    info->warehouse = atoi(row[3]);
    info->status    = atoi(row[4]);
    info->mode      = atoi(row[5]);
	strcpy(info->name,     row[1]);
}   
//  格式化 统计信息
void formatStatisticsCall(char **row, void *outdata)
{
    StatisticsModel *info = outdata;

    info->id                   = atoi(row[0]);
    info->warehouse            = atoi(row[1]);
    info->abnormal_temperature = atoi(row[3]);
    info->min_temperature      = atof(row[4]);
    info->max_temperature      = atof(row[5]);
    info->abnormal_humidity    = atoi(row[6]);
    info->min_humidity         = atof(row[7]);
    info->max_humidity         = atof(row[8]);
    info->abnormal_illuminance = atoi(row[9]);
    info->min_illuminance      = atof(row[10]);
    info->max_illuminance      = atof(row[11]);
	strcpy(info->date,                row[2]);
}   

