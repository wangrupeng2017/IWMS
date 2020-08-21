/************************************************
 * Name: store_models.h
 * Date: 2020-08-20
 * Description: 数据库模型定义
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-20
 ************************************************/

// 参数类型枚举
typedef enum ParamType {
    TypeTemperature  = 1,  //温度
    TypeHumidity     = 2,  //湿度
    TypeIllumination = 3,  //光照
} ParamType;

// 设备类型枚举
typedef enum DeviceType {
    TypeBuzzer = 1,     //蜂鸣器
    TypeLight  = 2,     //照明灯
    TypeFan    = 3,     //风扇
} DeviceType;

// 工作状态枚举
typedef enum DeviceWorkType {
    TypeAutomatic = 1,  //自动
    TypeManual    = 2,  //手动
} DeviceWorkType;


// 参数配置表
typedef struct ParamConfigModel {
    int   id;           //编号
    int   type;         //类型(温度/湿度/..)
    float min;          //最小值
    float max;          //最大值
    int   alarm;        //报警
    int   automation;   //自动控制
    int   status;       //状态
} ParamConfigModel;

// 温度/湿度/光照 数据表
typedef struct CommonValueModel {
    int   id;           //编号
    char  time[20];     //时间
    float value;        //值
    int   status;       //状态
    char  message[100]; //状态描述
} CommonValueModel;

// 设备状态表
typedef struct DeviceStatusModel {
    int   id;           //设备编号
    char  name[40];     //设备名称
    int   type;         //设备类型
    int   warehouse;    //所在仓库
    int   status;       //工作状态(关, 开/1档, ...)
    int   mode;         //工作模式(自动, 主动)
} DeviceStatusModel;

// 统计信息表
typedef struct StatisticsModel {
    int   id;                     //编号
    int   warehouse;              //仓库号
    char  date[20];               //日期
    int   abnormal_temperature;   //温度异常次数
    float min_temperature;        //最小温度值
    float max_temperature;        //最大温度值
    int   abnormal_humidity;      //湿度异常次数
    float min_humidity;           //最小湿度值
    float max_humidity;           //最大湿度值
    int   abnormal_illuminance;   //光照异常次数
    float min_illuminance;        //最小光照值
    float max_illuminance;        //最大光照值
} StatisticsModel;



