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
#include "cgi_params.h"
#include "cgi_response.h"


//  业务处理
int businessHandler(void *req, int reqlen, void **out, int *outlen);

int cgiMain()
{
	void *req  = NULL;
	int reqlen = 0;
	//  接收参数 仓库id
	int ret = receiveWarehouseId(&req, &reqlen);
	if (ret != Success) goto ErrorLabel1;

	void *res  = NULL;
	int reslen = 0;
	//  业务处理
	ret = businessHandler(req, reqlen, &res, &reslen);
	if (ret != Success) goto ErrorLabel2;

	//  响应结果 实时走势数据
	ret = sendRealtimeTrend(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendRealtimeTrend(NULL, 0);
	return 0;
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
	int rows = queryRecentlyTemperature(time(NULL), temperature);
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

