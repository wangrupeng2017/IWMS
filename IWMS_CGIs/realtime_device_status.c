/************************************************
 * Name: realtime_device_status.c 
 * Date: 2020-08-20
 * Description: 实时设备参数cgi
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
	WebDeviceStatus res = {
		.buzzer = 0,
		.light  = 0,
		.fan    = 0,
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
	WebDeviceStatus *res = data;
	
	cJSON *json = cJSON_CreateObject();
	cJSON_AddItemToObject(json, "buzzer", cJSON_CreateNumber(res->buzzer));
	cJSON_AddItemToObject(json, "light",   cJSON_CreateNumber(res->light));
	cJSON_AddItemToObject(json, "fan",   cJSON_CreateNumber(res->fan));
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

