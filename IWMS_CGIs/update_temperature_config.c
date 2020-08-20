/************************************************
 * Name: update_temperature_config.c 
 * Date: 2020-08-20
 * Description: 更新仓库温度配置cgi
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
	WarehouseConfig *req = NULL;
	int reqlen      = 0;
	int ret = getParameter((void **)&req, &reqlen);
	if (ret != Success)
		goto ErrorLabel1;

	//************
	// 执行更新数据库操作
	//************

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
	
	// 取出消息内容字符串
	char *buff = malloc(cgiContentLength+1);
	if (fgets(buff, cgiContentLength+1, cgiIn) == NULL)
		return ErrorReadContent; 

	// 转换成JSON对象
	cJSON *json = cJSON_Parse(buff);
	if (NULL == json) 
		return ErrorJsonParse;

	// 解析JSON赋值给对应结构体
	WarehouseConfig* req = malloc(sizeof(WarehouseConfig));
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;
	req->min = cJSON_GetObjectItem(json, "min")->valuedouble;
	req->max = cJSON_GetObjectItem(json, "max")->valuedouble;
	req->alarm = cJSON_GetObjectItem(json, "alarm")->valueint;
	req->automation = cJSON_GetObjectItem(json, "automation")->valueint;

	// 释放空间
	free(buff);
	cJSON_Delete(json);

	*out = req;	
	return Success;
}

/*
 * function:    响应结果
 * @param [ in] 返回的结果
 * @return      0:响应成功，!0:响应错误
 */
int sendResult(void *data)
{
	cJSON *json = cJSON_CreateNull();
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

