/************************************************
 * Name: cgi_params.h
 * Date: 2020-08-22
 * Description: cgi参数接收方法
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-22
 ************************************************/

//  接收参数 仓库id
int receiveWarehouseId(void **out, int *outlen);
//  接收参数 仓库参数配置(温度/湿度/光照)
int receiveWarehouseConfig(void **out, int *outlen);
//  接收参数 仓库历史
int receiveWarehouseHistory(void **out, int *outlen);
//  接收参数 仓库设备状态
int receiveWarehouseDevice(void **out, int *outlen);
