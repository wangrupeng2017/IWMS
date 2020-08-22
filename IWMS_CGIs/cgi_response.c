/************************************************
 * Name: cgi_response.c
 * Date: 2020-08-22
 * Description: cgi响应结果方法
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
 * function:    响应结果 实时参数配置(温度/湿度/光照)
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendRealtimeParam(void *res, int reslen)
{
	cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebRealtimeParam *data = res;
		json = cJSON_CreateObject();
		cJSON_AddItemToObject(json, "value",      cJSON_CreateNumber(data->value));
		cJSON_AddItemToObject(json, "min",        cJSON_CreateNumber(data->min));
		cJSON_AddItemToObject(json, "max",        cJSON_CreateNumber(data->max));
		cJSON_AddItemToObject(json, "alarm",      cJSON_CreateNumber(data->alarm));
		cJSON_AddItemToObject(json, "automation", cJSON_CreateNumber(data->automation));
		cJSON_AddItemToObject(json, "status",     cJSON_CreateNumber(data->status));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

/***************************************
 * function:    响应结果 设备状态(蜂鸣器/照明灯/风扇)
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendDeviceStatus(void *res, int reslen)
{
    cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebDeviceStatus *data = res;
		json = cJSON_CreateObject();
		cJSON_AddItemToObject(json, "buzzer", cJSON_CreateNumber(data->buzzer));
		cJSON_AddItemToObject(json, "light",  cJSON_CreateNumber(data->light));
		cJSON_AddItemToObject(json, "fan",    cJSON_CreateNumber(data->fan));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

/***************************************
 * function:    响应结果 NULL
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendNULL(void *res, int reslen)
{
    cJSON *json    = cJSON_CreateNull();
	char *json_str = cJSON_Print(json);

	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

/***************************************
 * function:    响应结果 实时走势数据
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendRealtimeTrend(void *res, int reslen)
{
	cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebRealtimeTrend *data = res;
		json = cJSON_CreateObject();
	    cJSON_AddItemToObject(json, "time",        cJSON_CreateString(data->time));
	    cJSON_AddItemToObject(json, "temperature", cJSON_CreateNumber(data->temperature));
	    cJSON_AddItemToObject(json, "humidity",    cJSON_CreateNumber(data->humidity));
	    cJSON_AddItemToObject(json, "illuminance", cJSON_CreateNumber(data->illuminance));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

/***************************************
 * function:    响应结果 异常统计信息
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendAbnormalStatistics(void *res, int reslen)
{
	cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebAbnormalStatistics *data = res;
		json = cJSON_CreateObject();
		cJSON_AddItemToObject(json, "abnormalTemperature", cJSON_CreateNumber(data->abnormalTemperature));
		cJSON_AddItemToObject(json, "minTemperature",      cJSON_CreateNumber(data->minTemperature));
		cJSON_AddItemToObject(json, "maxTemperature",      cJSON_CreateNumber(data->maxTemperature));
		cJSON_AddItemToObject(json, "abnormalHumidity",    cJSON_CreateNumber(data->abnormalHumidity));
		cJSON_AddItemToObject(json, "minHumidity",         cJSON_CreateNumber(data->minHumidity));
		cJSON_AddItemToObject(json, "maxHumidity",         cJSON_CreateNumber(data->maxHumidity));
		cJSON_AddItemToObject(json, "abnormalIlluminance", cJSON_CreateNumber(data->abnormalIlluminance));
		cJSON_AddItemToObject(json, "minIlluminance",      cJSON_CreateNumber(data->minIlluminance));
		cJSON_AddItemToObject(json, "maxIlluminance",      cJSON_CreateNumber(data->maxIlluminance));
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}

/***************************************
 * function:    响应结果 历史走势数据
 * @param [ in] res 响应数据
 * @param [ in] reslen 响应数据大小
 * @return      0:响应成功，!0:响应错误
 */
int sendHistoryTrend(void *res, int reslen)
{
	cJSON *json = NULL;
	if (res == NULL)
	{
		json = cJSON_CreateNull();
	}
	else
	{
		WebRealtimeTrend *datas = res;
		json = cJSON_CreateArray();
		int i = 0;
		for (i=0; i<(reslen/sizeof(WebRealtimeTrend)); i++)
		{
			cJSON *item = cJSON_CreateObject();
			cJSON_AddItemToObject(item, "time",        cJSON_CreateString((datas+i)->time));
			cJSON_AddItemToObject(item, "temperature", cJSON_CreateNumber((datas+i)->temperature));
			cJSON_AddItemToObject(item, "humidity",    cJSON_CreateNumber((datas+i)->humidity));
			cJSON_AddItemToObject(item, "illuminance", cJSON_CreateNumber((datas+i)->illuminance));
			cJSON_AddItemToArray(json, item);
		}
	}
	
	char *json_str = cJSON_Print(json);
	cgiHeaderContentType("application/json");
	fputs(json_str, cgiOut);

	free(json_str);
	return Success;
}
