/************************************************
 * Name: realtime_trend_data.c 
 * Date: 2020-08-20
 * Description: 实时走势数据cgi
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

	//  查询实时温度数据
	CommonValueModel *temperature = calloc(sizeof(CommonValueModel), 1);
	int rows = queryRecentlyHumidity(time(NULL), temperature);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }

	//  查询实时湿度数据
	CommonValueModel *humidity = calloc(sizeof(CommonValueModel), 1);
	rows = queryRecentlyHumidity(time(NULL), humidity);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL3; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL3; }

	//  查询实时光照数据
	CommonValueModel *illuminance = calloc(sizeof(CommonValueModel), 1);
	rows = queryRecentlyIllumination(time(NULL), illuminance);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL4; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL4; }

	//  格式化响应数据
	WebRealtimeTrend *res = calloc(sizeof(WebRealtimeTrend), 1);
	strcpy(res->time,  temperature->time);
	res->temperature = temperature->value;
	res->humidity    = humidity->value;
	res->illuminance = illuminance->value;

	//  传出响应数据
	*out    = res;
	*outlen = sizeof(WebRealtimeTrend);
	ret     = Success;

ERROR_LABEL4:
	free(illuminance);
ERROR_LABEL3:
	free(humidity);
ERROR_LABEL2:
	free(temperature);
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
		WebRealtimeTrend *data = res;
		json = cJSON_CreateObject();
	    cJSON_AddItemToObject(json, "time",        cJSON_CreateString(data->time));
	    cJSON_AddItemToObject(json, "temperature", cJSON_CreateNumber(data->temperature));
	    cJSON_AddItemToObject(json, "humidity",    cJSON_CreateNumber(data->humidity));
	    cJSON_AddItemToObject(json, "illuminance", cJSON_CreateNumber(data->illuminance));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

