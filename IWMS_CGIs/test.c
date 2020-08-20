/************************************************
 * Name: test.c 
 * Date: 2020-08-20
 * Description: 测试cgi
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
	WebRealtimeParam res = {
		.value  = 29.9,
		.min    = 10.0,
		.max    = 40.0,
		.alarm  = 0,
		.automation = 0,
		.status = 0,
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
	WebRealtimeParam *res = data;
	
	cJSON *json = cJSON_CreateObject();
	cJSON_AddItemToObject(json, "value", cJSON_CreateNumber(res->value));
	cJSON_AddItemToObject(json, "min",   cJSON_CreateNumber(res->min));
	cJSON_AddItemToObject(json, "max",   cJSON_CreateNumber(res->max));
	cJSON_AddItemToObject(json, "alarm", cJSON_CreateNumber(res->alarm));
	cJSON_AddItemToObject(json, "automation", cJSON_CreateNumber(res->automation));
	cJSON_AddItemToObject(json, "status", cJSON_CreateNumber(res->status));
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

