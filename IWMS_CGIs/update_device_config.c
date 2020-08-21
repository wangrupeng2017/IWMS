/************************************************
 * Name: update_device_config.c 
 * Date: 2020-08-20
 * Description: 更新仓库设备配置cgi
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
	WarehouseDevice* req = calloc(sizeof(WarehouseDevice), 1);
	req->warehouse_id = cJSON_GetObjectItem(json, "warehouse_id")->valueint;
	req->buzzer       = cJSON_GetObjectItem(json, "buzzer")->valueint;
	req->light        = cJSON_GetObjectItem(json, "light")->valueint;
	req->fan          = cJSON_GetObjectItem(json, "fan")->valueint;

	//  传出参数值
	*out = req;	
	*outlen = sizeof(WarehouseDevice);
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

	//  仓库id
	uint warehouse_id = ((WarehouseDevice*)req)->warehouse_id;
	//  查询蜂鸣器设备状态
	DeviceStatusModel *buzzer = calloc(sizeof(DeviceStatusModel), 1);
	int rows = queryDeviceStatus(warehouse_id, TypeBuzzer, buzzer);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }
	buzzer->status = ((WarehouseDevice*)req)->buzzer;
	buzzer->mode   = TypeManual;
	//  更新蜂鸣器设备状态
	ret = modifyDeviceStatus(buzzer);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }

	//  查询照明灯设备状态
	DeviceStatusModel *light = calloc(sizeof(DeviceStatusModel), 1);
	rows = queryDeviceStatus(warehouse_id, TypeLight, light);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL3; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL3; }
	light->status = ((WarehouseDevice*)req)->light;
	light->mode   = TypeManual;
	//  更新照明灯设备状态
	ret = modifyDeviceStatus(light);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL3; }

	//  查询风扇设备状态
	DeviceStatusModel *fan = calloc(sizeof(DeviceStatusModel), 1);
	rows = queryDeviceStatus(warehouse_id, TypeFan, fan);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL4; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL4; }
	fan->status = ((WarehouseDevice*)req)->fan;
	fan->mode   = TypeManual;
	//  更新风扇设备状态
	ret = modifyDeviceStatus(fan);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL4; }


	//  操作成功
	ret = Success;
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

