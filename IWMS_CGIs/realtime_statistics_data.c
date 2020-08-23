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
	//  接收参数 仓库id
	int ret = receiveWarehouseId(&req, &reqlen);
	if (ret != Success) goto ErrorLabel1;

	void *res  = NULL;
	int reslen = 0;
	//  业务处理
	ret = businessHandler(req, reqlen, &res, &reslen);
	if (ret != Success) goto ErrorLabel2;

	//  响应结果
	ret = sendAbnormalStatistics(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendAbnormalStatistics(NULL, 0);
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

	//  获取今天日期 YYYY-MM-DD
	char now_date[20] = "";
	timestampToDatestr(time(NULL), now_date);
	//  获取仓库号
	uint warehouse_id = ((WarehouseId*)req)->warehouse_id;

	//  查询统计信息
	StatisticsModel *statistics = calloc(sizeof(StatisticsModel), 1);
	int rows = queryStatistics(warehouse_id, now_date, statistics);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }

	//  格式化响应数据
	WebAbnormalStatistics *res = calloc(sizeof(WebAbnormalStatistics), 1);
	res->abnormalTemperature = statistics->abnormal_temperature;
	res->minTemperature      = statistics->min_temperature;
	res->maxTemperature      = statistics->max_temperature;
	res->abnormalHumidity    = statistics->abnormal_humidity;
	res->minHumidity         = statistics->min_humidity;
	res->maxHumidity         = statistics->max_humidity;
	res->abnormalIlluminance = statistics->abnormal_illuminance;
	res->minIlluminance      = statistics->min_illuminance;
	res->maxIlluminance      = statistics->max_illuminance;

	//  传出响应数据
	*out    = res;
	*outlen = sizeof(WebAbnormalStatistics);
	ret     = Success;

ERROR_LABEL2:
	free(statistics);
ERROR_LABEL1:
    closeSQL();
	return ret;
}
