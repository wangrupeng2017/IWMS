/************************************************
 * Name: update_humidity_config.c 
 * Date: 2020-08-20
 * Description: 更新仓库湿度配置cgi
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

	//  查询湿度配置
	WarehouseConfig *config = req;
	ParamConfigModel *model = calloc(sizeof(ParamConfigModel), 1);
	int rows = queryParamConfig(TypeHumidity, model);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }

	//  更新湿度配置
	model->min        = config->min;
	model->max        = config->max;
	model->alarm      = config->alarm;
	model->automation = config->automation;
	ret = modifyParamConfig(model);
	if (ret < 0)  { ret=ErrorModifySQL; goto ERROR_LABEL2; }

	//  操作成功
	ret = Success;
ERROR_LABEL2:
	free(model);
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
	cJSON *json    = cJSON_CreateNull();
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

