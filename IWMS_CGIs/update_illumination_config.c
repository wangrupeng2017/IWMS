/************************************************
 * Name: update_illumination_config.c 
 * Date: 2020-08-20
 * Description: 更新仓库光照配置cgi
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
	//  接收参数 仓库参数配置(光照)
	int ret = receiveWarehouseConfig(&req, &reqlen);
	if (ret != Success) goto ErrorLabel1;

	void *res  = NULL;
	int reslen = 0;
	//  业务处理
	ret = businessHandler(req, reqlen, &res, &reslen);
	if (ret != Success) goto ErrorLabel2;

	//  响应结果
	ret = sendNULL(res, reslen);
	if (ret != Success) goto ErrorLabel3;

	free(req);
	free(res);
	return 0;
ErrorLabel3:
ErrorLabel2:
	free(req);
ErrorLabel1:
	sendNULL(NULL, 0);
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

	//  查询光照配置
	WarehouseConfig *config = req;
	ParamConfigModel *model = calloc(sizeof(ParamConfigModel), 1);
	int rows = queryParamConfig(TypeIllumination, model);
	if (rows < 0)  { ret=ErrorQuerySQL; goto ERROR_LABEL2; }
	if (rows == 0) { ret=Success;       goto ERROR_LABEL2; }

	//  更新光照配置
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
