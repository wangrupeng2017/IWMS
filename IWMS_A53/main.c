/************************************************
 * Name: main.c 
 * Date: 2020-08-21
 * Description: 负责控制M0, 
 *              读取(温度/湿度/光照)等信息, 
 *              控制(蜂鸣器,照明灯,风扇)等设备.
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-21
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "common_base.h"
#include "common_time.h"
#include "store_models.h"
#include "store.h"
#include "value_dispose.h"

//  采集/控制间隔(单位:s)
#define GATHER_INTERVAL  1
//  仓库号
#define WAREHOUSE_ID  1


//  温度采集任务
void *gatherTemperatureTask(void *arg);
//  湿度采集任务
void *gatherHumidityTask(void *arg);
//  光照采集任务
void *gatherIlluminationTask(void *arg);
//  蜂鸣器控制任务
void *buzzerControlTask(void *arg);
//  照明灯控制任务
void *lightControlTask(void *arg);
//  风扇控制任务
void *fanControlTask(void *arg);

int main (int argc, const char *argv[])
{
    //  创建数据库实例对象
    sqlite3 *db_instance = NULL;
	int ret = initSQL(&db_instance);
    TRY_ERROR(ret!=0, "initSQL:", return FuncError);

    //  创建数据表
    ret = createTables();
    TRY_ERROR(ret!=0, "createTables:", return FuncError);
    //  添加默认数据
    ret = addDefaultData();
    TRY_ERROR(ret!=0, "addDefaultData:", return FuncError);

    pthread_t temperature_tid  = 0;
    pthread_t humidity_tid     = 0;
    pthread_t illumination_tid = 0;
    pthread_t buzzer_tid       = 0;
    pthread_t light_tid        = 0;
    pthread_t fan_tid          = 0;

    //  创建读取温度的线程
    ret = pthread_create(&temperature_tid, NULL, gatherTemperatureTask, NULL);
    TRY_PERROR(ret!=0, "创建温度采集线程:", return FuncError);
    //  创建读取湿度的线程
    ret = pthread_create(&humidity_tid, NULL, gatherHumidityTask, NULL);
    TRY_PERROR(ret!=0, "创建湿度采集线程:", return FuncError);
    //  创建读取光照的线程
    ret = pthread_create(&illumination_tid, NULL, gatherIlluminationTask, NULL);
    TRY_PERROR(ret!=0, "创建光照采集线程:", return FuncError);
    //  创建控制蜂鸣器的线程
    ret = pthread_create(&buzzer_tid, NULL, buzzerControlTask, NULL);
    TRY_PERROR(ret!=0, "创建蜂鸣器控制线程:", return FuncError);
    //  创建控制照明灯的线程
    ret = pthread_create(&light_tid, NULL, lightControlTask, NULL);
    TRY_PERROR(ret!=0, "创建照明控制线程:", return FuncError);
    //  创建控制风扇的线程
    ret = pthread_create(&fan_tid, NULL, fanControlTask, NULL);
    TRY_PERROR(ret!=0, "创建风扇控制线程:", return FuncError);
    
    //  阻塞等待 温度采集线程 结束
    pthread_join(temperature_tid, NULL);
    puts("!!! 温度采集线程 结束");
    //  阻塞等待 湿度采集线程 结束
    pthread_join(humidity_tid, NULL);
    puts("!!! 湿度采集线程 结束");
    //  阻塞等待 光照采集线程 结束
    pthread_join(illumination_tid, NULL);
    puts("!!! 光照采集线程 结束");
    //  阻塞等待 蜂鸣器控制线程 结束
    pthread_join(buzzer_tid, NULL);
    puts("!!! 蜂鸣器控制线程 结束");
    //  阻塞等待 照明控制线程 结束
    pthread_join(light_tid, NULL);
    puts("!!! 照明控制线程 结束");
    //  阻塞等待 风扇控制线程 结束
    pthread_join(fan_tid, NULL);
    puts("!!! 风扇控制线程 结束");


    //  关闭数据库
    closeSQL();
    return 0;
}


//  温度采集任务
void *gatherTemperatureTask(void *arg)
{
    int rows = 0;
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送温度采集命令
        //************************
        //  这里使用随机数模拟采集的温度: -10.0 ~ 40.0 
        //************************
        CommonValueModel temperature = { .message="温度" };
        temperature.value = ((float)(rand() % 500)) / 10 - 10; 
        
        //  查询温度配置, 检查温度是否异常
        ParamConfigModel config;
        rows = queryParamConfig(TypeTemperature, &config);
        if (rows < 0)  { puts("采集温度::查询温度配置出错"); continue; }
        if (rows > 0)  disposeValueStatus(&temperature, config.min, config.max);

        //  查询统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        if (rows < 0)  { puts("采集温度::查询统计数据出错"); continue; }
        if (rows == 0)  
        { 
            statistics.warehouse=WAREHOUSE_ID;
            strcpy(statistics.date, date);
        }

        //  更新统计信息值
        disposeStatisticsValue(&temperature, &statistics.abnormal_temperature, \
                               &statistics.min_temperature, &statistics.max_temperature);
        //  如果上一次采集温度就存在异常 本次采集不累计异常计数
        if (temperature.status && lastTimeValue.status) 
            statistics.abnormal_temperature -= 1;


        //  更新温度当前状态配置
        config.status = temperature.status;
        modifyParamConfig(&config);
        //  存储温度采集数据
        addRealtimeTemperature(&temperature);
        //  更新温度统计计数
        modifyStatistics(&statistics);

        //  记录本次的温度值为上次的温度值
        lastTimeValue = temperature;
    }

    return NULL;
}
//  湿度采集任务
void *gatherHumidityTask(void *arg) 
{
    int rows = 0;
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送湿度采集命令
        //************************
        //  这里使用随机数模拟采集的湿度: -0.0 ~ 70.0 
        CommonValueModel humidity = { .message="湿度" };
        humidity.value = ((float)(rand() % 700)) / 10; 
        
        //  查询湿度配置, 检查湿度是否异常
        ParamConfigModel config;
        rows = queryParamConfig(TypeHumidity, &config);
        if (rows < 0)  { puts("采集湿度::查询湿度配置出错"); continue; }
        if (rows > 0)  disposeValueStatus(&humidity, config.min, config.max);

        //  查询统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        if (rows < 0)  { puts("采集湿度::查询统计数据出错"); continue; }
        if (rows == 0)  
        { 
            statistics.warehouse=WAREHOUSE_ID;
            strcpy(statistics.date, date);
        }

        //  更新统计信息值
        disposeStatisticsValue(&humidity, &statistics.abnormal_humidity, \
                               &statistics.min_humidity, &statistics.max_humidity);
        //  如果上一次采集湿度就存在异常 本次采集不累计异常计数
        if (humidity.status && lastTimeValue.status) 
            statistics.abnormal_humidity -= 1;

        //  更新湿度当前状态配置
        config.status = humidity.status;
        modifyParamConfig(&config);
        //  存储湿度采集数据
        addRealtimeHumidity(&humidity);
        //  更新湿度统计计数
        modifyStatistics(&statistics);

        //  记录本次的湿度值为上次的温度值
        lastTimeValue = humidity;
    }

    return NULL;
}
//  光照采集任务
void *gatherIlluminationTask(void *arg)
{
    int rows = 0;
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送光照采集命令
        //************************
        //  这里使用随机数模拟采集的光照: -0.0 ~ 100.0 
        CommonValueModel illumination = { .message="光照" };
        illumination.value = ((float)(rand() % 1000)) / 10; 
        

        //  查询光照配置, 检查光照是否异常
        ParamConfigModel config;
        rows = queryParamConfig(TypeIllumination, &config);
        if (rows < 0)  { puts("采集光照::查询光照配置出错"); continue; }
        if (rows > 0)  disposeValueStatus(&illumination, config.min, config.max);

        //  查询统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        if (rows < 0)  { puts("采集光照::查询统计数据出错"); continue; }
        if (rows == 0)  
        { 
            statistics.warehouse=WAREHOUSE_ID;
            strcpy(statistics.date, date);
        }

        //  更新统计信息值
        disposeStatisticsValue(&illumination, &statistics.abnormal_illuminance, \
                               &statistics.min_illuminance, &statistics.max_illuminance);
        //  如果上一次采集光照就存在异常 本次采集不累计异常计数
        if (illumination.status && lastTimeValue.status) 
            statistics.abnormal_illuminance -= 1;

        //  更新光照当前状态配置
        config.status = illumination.status;
        modifyParamConfig(&config);
        //  存储光照采集数据
        addRealtimeIllumination(&illumination);
        //  更新光照统计计数
        modifyStatistics(&statistics);

        //  记录本次的光照值为上次的光照值
        lastTimeValue = illumination;
    }

    return NULL;
}
//  蜂鸣器控制任务
void *buzzerControlTask(void *arg)
{
    ParamConfigModel temperature  = {0};
    ParamConfigModel humidity     = {0};
    ParamConfigModel illumination = {0};
    DeviceStatusModel buzzer      = {0};

    while(1)
    {
        sleep(GATHER_INTERVAL);

        queryParamConfig(TypeTemperature,  &temperature);
        queryParamConfig(TypeHumidity,     &humidity);
        queryParamConfig(TypeIllumination, &illumination);
        queryDeviceStatus(WAREHOUSE_ID, TypeBuzzer, &buzzer);

        //  判断设备状态是否需要改变, 在需要改变时向M0发送命令 并 更新设备状态
        int alarm = 0;
        if (temperature.status  && temperature.alarm)  alarm = 1;
        if (humidity.status     && humidity.alarm)     alarm = 1;
        if (illumination.status && illumination.alarm) alarm = 1;

        //  需要报警但蜂鸣器没有打开, 自动打开蜂鸣器
        if (alarm && (!buzzer.status)) 
        {
            buzzer.status = 1;
            buzzer.mode   = TypeAutomatic;
        }
        //  不需要报警但蜂鸣器打开, 并且不是用户主动操作, 关闭蜂鸣器
        else if (!alarm && buzzer.status && (buzzer.mode==TypeAutomatic))
            buzzer.status = 0;

        //  更新设备状态
        modifyDeviceStatus(&buzzer);

        //************************
        //  根据状态发送命令给M0
        //************************
    }

    return NULL;
}
//  照明灯控制任务
void *lightControlTask(void *arg)
{
    ParamConfigModel illumination = {0};
    DeviceStatusModel light      = {0};

    while(1)
    {
        sleep(GATHER_INTERVAL);

        queryParamConfig(TypeIllumination, &illumination);
        queryDeviceStatus(WAREHOUSE_ID, TypeLight, &light);

        //  判断设备状态是否需要改变, 在需要改变时向M0发送命令 并 更新设备状态
        int automation = 0;
        if (illumination.status<0 && illumination.automation) automation = 1;

        //  照明太低但照明灯没有打开, 自动打开照明灯
        if (automation && (!light.status)) 
        {
            light.status = 1;
            light.mode   = TypeAutomatic;
        }
        //  不需要照明但照明已打开, 并且不是用户主动操作, 关闭照明灯
        else if (!automation && light.status && (light.mode==TypeAutomatic))
            light.status = 0;

        //  更新设备状态
        modifyDeviceStatus(&light);

        //************************
        //  根据状态发送命令给M0
        //************************
    }

    return NULL;
}
//  风扇控制任务
void *fanControlTask(void *arg)
{
    ParamConfigModel temperature  = {0};
    ParamConfigModel humidity     = {0};
    DeviceStatusModel fan         = {0};

    while(1)
    {
        sleep(GATHER_INTERVAL);

        queryParamConfig(TypeTemperature,  &temperature);
        queryParamConfig(TypeHumidity,     &humidity);
        queryDeviceStatus(WAREHOUSE_ID, TypeFan, &fan);

        //  判断设备状态是否需要改变, 在需要改变时向M0发送命令 并 更新设备状态
        int automation = 0;
        if (temperature.status>0  && temperature.automation)  automation = 1;
        if (humidity.status>0     && humidity.automation)     automation = 1;

        //  需要报警但蜂鸣器没有打开, 自动打开蜂鸣器
        if (automation && (!fan.status)) 
        {
            fan.status = 1;
            fan.mode   = TypeAutomatic;
        }
        //  不需要报警但蜂鸣器打开, 并且不是用户主动操作, 关闭蜂鸣器
        else if (!automation && fan.status && (fan.mode==TypeAutomatic))
            fan.status = 0;

        //  更新设备状态
        modifyDeviceStatus(&fan);

        //************************
        //  根据状态发送命令给M0
        //************************
    }

    return NULL;
}

