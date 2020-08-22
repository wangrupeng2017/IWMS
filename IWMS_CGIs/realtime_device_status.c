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

	//  响应结果
	ret = sendDeviceStatus(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendDeviceStatus(NULL, 0);
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

	//  仓库id
	uint warehouse_id = ((WarehouseId*)req)->warehouse_id;
	//  查询蜂鸣器设备状态
	DeviceStatusModel *buzzer = calloc(sizeof(DeviceStatusModel), 1);
	int rows = queryDeviceStatus(warehouse_id, TypeBuzzer, buzzer);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }

	//  查询照明灯设备状态
	DeviceStatusModel *light = calloc(sizeof(DeviceStatusModel), 1);
	rows = queryDeviceStatus(warehouse_id, TypeLight, light);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL3; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL3; }

	//  查询风扇设备状态
	DeviceStatusModel *fan = calloc(sizeof(DeviceStatusModel), 1);
	rows = queryDeviceStatus(warehouse_id, TypeFan, fan);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL4; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL4; }

	//  格式化响应数据
	WebDeviceStatus *res = calloc(sizeof(WebDeviceStatus), 1);
	res->buzzer = buzzer->status;
	res->light  = light->status;
	res->fan    = fan->status;

	//  传出响应数据
	*out    = res;
	*outlen = sizeof(WebDeviceStatus);
	ret     = Success;

ERROR_LABEL4:
	free(fan);
ERROR_LABEL3:
	free(light);
ERROR_LABEL2:
	free(buzzer);
ERROR_LABEL1:
    closeSQL();
	return ret;
}

