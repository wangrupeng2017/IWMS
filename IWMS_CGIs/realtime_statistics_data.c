/************************************************
 * Name: realtime_statistics_data.c 
 * Date: 2020-08-20
 * Description: 实时统计数据cgi
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-20
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cgic.h"
#include "cJSON.h"
#include "web_session_struct.h"
#include "store.h"
#include "common_time.h"

//  接收参数
int receiveParameter(void **out, int *outlen);
//  业务处理
int businessHandler(void *req, int reqlen, void **out, int *outlen);
//  响应结果
int sendResult(void *res, int reslen);

int cgiMain()
{
	void *req  = NULL;
	int reqlen = 0;
	//  接收参数
	int ret = receiveParameter(&req, &reqlen);
	if (ret != Success) goto ErrorLabel1;

	void *res  = NULL;
	int reslen = 0;
	//  业务处理
	ret = businessHandler(req, reqlen, &res, &reslen);
	if (ret != Success) goto ErrorLabel2;

	//  响应结果
	ret = sendResult(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendResult(NULL, 0);
	return 0;
}

/***************************************
 * function:    获取请求参数
 * @param [out] out 请求参数
 * @param [out] outlen 参数大小
 * @return      0:接受成功，!0:接受错误
 */
int receiveParameter(void **out, int *outlen)
{
	//  请求方式检查
	if (strcmp(cgiRequestMethod, "POST") != 0)
		return ErrorRequestMethod;
	
	int ret = 0;
	//  请求参数读取 (未处理,字符串格式)
	char *buff = calloc(cgiContentLength+1, 1);
	if (fgets(buff, cgiContentLength+1, cgiIn) == NULL)
	{
		ret = ErrorReadContent;
		goto ERROR_LABEL1;
	}

	//  参数解析
	cJSON *json = cJSON_Parse(buff);
	if (NULL == json)  { ret=ErrorJsonParse; goto ERROR_LABEL1; }
	WarehouseId  *req = calloc(sizeof(WarehouseId), 1);
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;

	//  传出参数值
	*out = req;	
	*outlen = sizeof(WarehouseId);
	ret = Success;

	//  释放过度内存
	cJSON_Delete(json);
ERROR_LABEL1:
	free(buff);
	return ret;
}
/***************************************
 * function:    业务处理方法
 * @param [ in] req 请求参数
 * @param [ in] reqlen 参数大小
 * @param [out] out 响应数据
 * @param [out] outlen 响应数据大小
 * @return      0:接受成功，!0:接受错误
 */
int businessHandler(void *req, int reqlen, void **out, int *outlen)
{
	sqlite3 *db_instance = NULL;
	//  创建数据库实例
	int ret = initSQL(&db_instance);
	if (ret != 0)  { ret=ErrorInitSQL;  goto ERROR_LABEL1; }

	//  获取今天日期 YYYY-MM-DD
	char now_date[20] = "";
	timestampToDatestr(time(NULL), now_date);
	//  获取仓库号
	uint warehouse_id = ((WarehouseId*)req)->warehouse_id;

	//  查询统计信息
	StatisticsModel *statistics = calloc(sizeof(StatisticsModel), 1);
	int rows = queryStatistics(warehouse_id, now_date, statistics);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }

	//  格式化响应数据
	WebAbnormalStatistics *res = calloc(sizeof(WebAbnormalStatistics), 1);
	res->abnormalTemperature = statistics->abnormal_temperature;
	res->minTemperature      = statistics->min_temperature;
	res->maxTemperature      = statistics->max_temperature;
	res->abnormalHumidity    = statistics->abnormal_humidity;
	res->minHumidity         = statistics->min_humidity;
	res->maxHumidity         = statistics->max_humidity;
	res->abnormalIlluminance = statistics->abnormal_illuminance;
	res->minIlluminance      = statistics->min_illuminance;
	res->maxIlluminance      = statistics->max_illuminance;

	//  传出响应数据
	*out    = res;
	*outlen = sizeof(WebAbnormalStatistics);
	ret     = Success;

ERROR_LABEL2:
	free(statistics);
ERROR_LABEL1:
    closeSQL();
	return ret;
}
/***************************************
 * function:    响应结果
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendResult(void *res, int reslen)
{
	cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebAbnormalStatistics *data = res;
		json = cJSON_CreateObject();
		cJSON_AddItemToObject(json, "abnormalTemperature", cJSON_CreateNumber(data->abnormalTemperature));
		cJSON_AddItemToObject(json, "minTemperature",      cJSON_CreateNumber(data->minTemperature));
		cJSON_AddItemToObject(json, "maxTemperature",      cJSON_CreateNumber(data->maxTemperature));
		cJSON_AddItemToObject(json, "abnormalHumidity",    cJSON_CreateNumber(data->abnormalHumidity));
		cJSON_AddItemToObject(json, "minHumidity",         cJSON_CreateNumber(data->minHumidity));
		cJSON_AddItemToObject(json, "maxHumidity",         cJSON_CreateNumber(data->maxHumidity));
		cJSON_AddItemToObject(json, "abnormalIlluminance", cJSON_CreateNumber(data->abnormalIlluminance));
		cJSON_AddItemToObject(json, "minIlluminance",      cJSON_CreateNumber(data->minIlluminance));
		cJSON_AddItemToObject(json, "maxIlluminance",      cJSON_CreateNumber(data->maxIlluminance));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}


