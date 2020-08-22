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

#include "cgic.h"
#include "cJSON.h"
#include "web_session_struct.h"
#include "store.h"
#include "common_time.h"
#include "cgi_params.h"
#include "cgi_response.h"

//  业务处理
int businessHandler(void *req, int reqlen, void **out, int *outlen);

int cgiMain()
{
	void *req  = NULL;
	int reqlen = 0;
	//  接收参数 仓库历史
	int ret = receiveWarehouseHistory(&req, &reqlen);
	if (ret != Success) goto ErrorLabel1;

	void *res  = NULL;
	int reslen = 0;
	//  业务处理
	ret = businessHandler(req, reqlen, &res, &reslen);
	if (ret != Success) goto ErrorLabel2;

	//  响应结果 历史走势数据
	ret = sendHistoryTrend(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendHistoryTrend(NULL, 0);
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

	//  历史日期
	char *history_date = ((WarehouseHistory*)req)->date;

	//  查询一整天60条温度记录
	CommonValueModel *temperatures = calloc(sizeof(CommonValueModel), 60);
	int rows = querySomedayTemperature(history_date, 60, temperatures);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	//  查询一整天60条湿度度记录
	CommonValueModel *humiditys = calloc(sizeof(CommonValueModel), 60);
	rows = querySomedayHumidity(history_date, 60, humiditys);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL3; }
	//  查询一整天60条光照记录
	CommonValueModel *illuminations = calloc(sizeof(CommonValueModel), 60);
	rows = querySomedayIllumination(history_date, 60, illuminations);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL4; }

	//  格式化响应数据
	WebRealtimeTrend *res = calloc(sizeof(WebRealtimeTrend), 60);
	int i = 0;
	for (i=0; i<60; i++)
	{
		strcpy(res[i].time, temperatures[i].time);
		res[i].temperature = temperatures[i].value;
		res[i].humidity    = humiditys[i].value;
		res[i].illuminance = illuminations[i].value;
	}

	//  传出响应数据
	*out    = res;
	*outlen = sizeof(WebRealtimeTrend)*60;
	ret     = Success;

ERROR_LABEL4:
	free(illuminations);
ERROR_LABEL3:
	free(humiditys);
ERROR_LABEL2:
	free(temperatures);
ERROR_LABEL1:
    closeSQL();
	return ret;
}

