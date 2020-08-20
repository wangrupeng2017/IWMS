/************************************************
 * Name: history_trend_data.c 
 * Date: 2020-08-20
 * Description: 历史走势数据cgi
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
int sendResult(void *data, int datasize);

int cgiMain()
{
	// 接收请求参数
	WarehouseHistory *req = NULL;
	int reqlen      = 0;
	int ret = getParameter((void **)&req, &reqlen);
	if (ret != Success)
		goto ErrorLabel1;

	//********************
	// 根据id+日期查询数据
	//********************

	// 返回结果
	WebRealtimeTrend res[3] = {
		[0] = {
			.time         = "18:29",
			.temperature  = 10.0,
			.temperature  = 40.0,
			.illuminance  = 100,
		},
		[1] = {
			.time         = "18:30",
			.temperature  = 10.0,
			.temperature  = 40.0,
			.illuminance  = 100,
		},
		[2] = {
			.time         = "18:31",
			.temperature  = 10.0,
			.temperature  = 40.0,
			.illuminance  = 100,
		}
	};
	sendResult(res, sizeof(res));
	return 0;
ErrorLabel1:
	sendResult(NULL, 0);
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
 * @param [ in] data: 返回的结果
 * @param [ in] datasize: 返回的结果大小
 * @return      0:响应成功，!0:响应错误
 */
int sendResult(void *data, int datasize)
{
	WebRealtimeTrend *res = data;
	cJSON *json = cJSON_CreateArray();

	int i = 0;
	for (i=0; i<(datasize/sizeof(WebRealtimeTrend)); i++)
	{
		cJSON *temp = cJSON_CreateObject();
		cJSON_AddItemToObject(temp, "time", cJSON_CreateString((res+i)->time));
		cJSON_AddItemToObject(temp, "temperature", cJSON_CreateNumber((res+i)->temperature));
		cJSON_AddItemToObject(temp, "humidity", cJSON_CreateNumber((res+i)->humidity));
		cJSON_AddItemToObject(temp, "illuminance", cJSON_CreateNumber((res+i)->illuminance));
		cJSON_AddItemToArray(json, temp);
	}

	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

