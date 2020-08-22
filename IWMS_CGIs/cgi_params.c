/************************************************
 * Name: cgi_params.c
 * Date: 2020-08-22
 * Description: cgi参数接收方法
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-22
 ************************************************/
#include <stdlib.h>
#include <string.h>
#include "web_session_struct.h"
#include "cgic.h"
#include "cJSON.h"

/***************************************
 * function:    获取请求参数 仓库id
 * @param [out] out 请求参数
 * @param [out] outlen 参数大小
 * @return      0:接受成功，!0:接受错误
 */
int receiveWarehouseId(void **out, int *outlen)
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
 * function:    接收参数 仓库参数配置(温度/湿度/光照)
 * @param [out] out 请求参数
 * @param [out] outlen 参数大小
 * @return      0:接受成功，!0:接受错误
 */
int receiveWarehouseConfig(void **out, int *outlen)
{
	if (strcmp(cgiRequestMethod, "POST") != 0)
		return ErrorRequestMethod;
	
	int ret = 0;
	// 取出消息内容字符串
	char *buff = calloc(cgiContentLength+1, 1);
	if (fgets(buff, cgiContentLength+1, cgiIn) == NULL)
	{
		ret = ErrorReadContent;
		goto ERROR_LABEL1;
	}

	// 转换成JSON对象
	cJSON *json = cJSON_Parse(buff);
	if (NULL == json)  { ret=ErrorJsonParse; goto ERROR_LABEL1; }

	// 解析JSON赋值给对应结构体
	WarehouseConfig* req = calloc(sizeof(WarehouseConfig), 1);
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;
	req->min          = cJSON_GetObjectItem(json, "min")->valuedouble;
	req->max          = cJSON_GetObjectItem(json, "max")->valuedouble;
	req->alarm        = cJSON_GetObjectItem(json, "alarm")->valueint;
	req->automation   = cJSON_GetObjectItem(json, "automation")->valueint;

	//  传出参数值
	*out = req;	
	*outlen = sizeof(WarehouseConfig);
	ret = Success;

	//  释放过度内存
	cJSON_Delete(json);
ERROR_LABEL1:
	free(buff);
	return ret;
}

/***************************************
 * function:    接收参数 仓库历史
 * @param [out] out 请求参数
 * @param [out] outlen 参数大小
 * @return      0:接受成功，!0:接受错误
 */
int receiveWarehouseHistory(void **out, int *outlen)
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
	WarehouseHistory *req = calloc(sizeof(WarehouseHistory), 1);
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;
	strcpy(req->date,   cJSON_GetObjectItem(json, "date")->valuestring);

	//  传出参数值
	*out = req;	
	*outlen = sizeof(WarehouseHistory);
	ret = Success;

	//  释放过度内存
	cJSON_Delete(json);
ERROR_LABEL1:
	free(buff);
	return ret;
}

/***************************************
 * function:    接收参数 仓库设备状态
 * @param [out] out 请求参数
 * @param [out] outlen 参数大小
 * @return      0:接受成功，!0:接受错误
 */
int receiveWarehouseDevice(void **out, int *outlen)
{
	if (strcmp(cgiRequestMethod, "POST") != 0)
		return ErrorRequestMethod;
	
	int ret = 0;
	// 取出消息内容字符串
	char *buff = calloc(cgiContentLength+1, 1);
	if (fgets(buff, cgiContentLength+1, cgiIn) == NULL)
	{
		ret = ErrorReadContent;
		goto ERROR_LABEL1;
	}

	// 转换成JSON对象
	cJSON *json = cJSON_Parse(buff);
	if (NULL == json)  { ret=ErrorJsonParse; goto ERROR_LABEL1; }

	// 解析JSON赋值给对应结构体
	WarehouseDevice* req = calloc(sizeof(WarehouseDevice), 1);
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;
	req->buzzer       = cJSON_GetObjectItem(json, "buzzer")->valueint;
	req->light        = cJSON_GetObjectItem(json, "light")->valueint;
	req->fan          = cJSON_GetObjectItem(json, "fan")->valueint;

	//  传出参数值
	*out = req;	
	*outlen = sizeof(WarehouseDevice);
	ret = Success;

	//  释放过度内存
	cJSON_Delete(json);
ERROR_LABEL1:
	free(buff);
	return ret;
}
