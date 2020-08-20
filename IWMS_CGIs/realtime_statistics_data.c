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
#include "web_session_struct.h"
#include "cgic.h"
#include "cJSON.h"

int getParameter(void **out, int *outlen);
int sendResult(void *data);

int cgiMain()
{
	// 接收请求参数
	WarehouseId *req = NULL;
	int reqlen      = 0;
	int ret = getParameter((void **)&req, &reqlen);
	if (ret != Success)
		goto ErrorLabel1;

	// 返回结果
	WebAbnormalStatistics res = {
		.abnormalTemperature = 5,
		.minTemperature      = 10.0,
		.maxTemperature      = 20.0,
		.abnormalHumidity    = 100,
		.minHumidity         = 20.9,
		.maxHumidity         = 98.7,
		.abnormalIlluminance = 4,
		.minIlluminance      = 29.8,
		.maxIlluminance      = 75.9
	};
	sendResult(&res);
	return 0;
ErrorLabel1:
	sendResult(NULL);
	return 0;	
}

/*
 * function:    获取请求参数
 * @param [out] out: 格式化后的参数
 * @param [out] outlen: 格式化后的参数
 * @return      0:接受成功，!0:接受错误
 */
int getParameter(void **out, int *outlen)
{
	if (strcmp(cgiRequestMethod, "POST") != 0)
		return ErrorRequestMethod;
	
	char *buff = malloc(cgiContentLength+1);
	if (fgets(buff, cgiContentLength+1, cgiIn) == NULL)
		return ErrorReadContent; 

	// +++++++++++++
	// JSON解析
	// +++++++++++++

	*out = (char **)buff;	
	return Success;
}

/*
 * function:    响应结果
 * @param [ in] 返回的结果
 * @return      0:响应成功，!0:响应错误
 */
int sendResult(void *data)
{
	WebAbnormalStatistics *res = data;
	
	cJSON *json = cJSON_CreateObject();
	cJSON_AddItemToObject(json, "abnormalTemperature", cJSON_CreateNumber(res->abnormalTemperature));
	cJSON_AddItemToObject(json, "minTemperature", cJSON_CreateNumber(res->minTemperature));
	cJSON_AddItemToObject(json, "maxTemperature", cJSON_CreateNumber(res->maxTemperature));
	cJSON_AddItemToObject(json, "abnormalHumidity", cJSON_CreateNumber(res->abnormalHumidity));
	cJSON_AddItemToObject(json, "minHumidity", cJSON_CreateNumber(res->minHumidity));
	cJSON_AddItemToObject(json, "maxHumidity", cJSON_CreateNumber(res->maxHumidity));
	cJSON_AddItemToObject(json, "abnormalIlluminance", cJSON_CreateNumber(res->abnormalIlluminance));
	cJSON_AddItemToObject(json, "minIlluminance", cJSON_CreateNumber(res->minIlluminance));
	cJSON_AddItemToObject(json, "maxIlluminance", cJSON_CreateNumber(res->maxIlluminance));
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

