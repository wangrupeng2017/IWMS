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
    // //  创建控制蜂鸣器的线程
    // ret = pthread_create(&buzzer_tid, NULL, buzzerControlTask, NULL);
    // TRY_PERROR(ret!=0, "创建蜂鸣器控制线程:", return FuncError);
    // //  创建控制照明灯的线程
    // ret = pthread_create(&light_tid, NULL, lightControlTask, NULL);
    // TRY_PERROR(ret!=0, "创建照明控制线程:", return FuncError);
    // //  创建控制风扇的线程
    // ret = pthread_create(&fan_tid, NULL, fanControlTask, NULL);
    // TRY_PERROR(ret!=0, "创建风扇控制线程:", return FuncError);
    
    //  阻塞等待 温度采集线程 结束
    pthread_join(temperature_tid, NULL);
    puts("!!! 温度采集线程 结束");
    //  阻塞等待 湿度采集线程 结束
    pthread_join(humidity_tid, NULL);
    puts("!!! 湿度采集线程 结束");
    //  阻塞等待 光照采集线程 结束
    pthread_join(illumination_tid, NULL);
    puts("!!! 光照采集线程 结束");
    // //  阻塞等待 蜂鸣器控制线程 结束
    // pthread_join(buzzer_tid, NULL);
    // puts("!!! 蜂鸣器控制线程 结束");
    // //  阻塞等待 照明控制线程 结束
    // pthread_join(light_tid, NULL);
    // puts("!!! 照明控制线程 结束");
    // //  阻塞等待 风扇控制线程 结束
    // pthread_join(fan_tid, NULL);
    // puts("!!! 风扇控制线程 结束");


    //  关闭数据库
    closeSQL();
    return 0;
}


//  温度采集任务
void *gatherTemperatureTask(void *arg)
{
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送温度采集命令
        //************************
        //  这里使用随机数模拟采集的温度: -10.0 ~ 40.0 
        CommonValueModel temperature = {0};
        temperature.value = ((float)(rand() % 500)) / 10 - 10; 
        
        int ret = 0;
        //  查询温度配置, 检查温度是否异常
        ParamConfigModel config;
        int rows = queryParamConfig(TypeTemperature, &config);
        if (rows == 1)
        {
            //  温度与最低检测温度比较
            if (temperature.value < config.min) 
            {
                temperature.status = (int)(temperature.value - config.min);
                strcpy(temperature.message, "温度过低");
            } 
            //  温度与最高检测温度比较
            else if (temperature.value > config.max)
            {
                temperature.status = (int)(temperature.value - config.max);
                strcpy(temperature.message, "温度过高");
                
                /**
                //  温度过高, 根据自动控制配置决定是否打开风扇
                if (config.automation)
                {
                    //  查询风扇现在的状态
                    DeviceStatusModel device;
                    rows = queryDeviceStatus(1, TypeFan, &device);
                    if (rows == 1)
                    {
                        //  风扇关闭状态, 打开风扇, 模式变为自动控制
                        if (device.status == 0)
                        {
                            //  先默认开启2档, 后期也可以根据温度自动变化
                            device.status = 2;  
                            device.mode   = TypeAutomatic; 
                            //  更新设备状态
                            ret = modifyDeviceStatus(&device);
                            if (ret!=0) { puts("更新风扇状态失败"); }
                        }
                    }
                }
                **/
            }
        }
        //  添加温度采集数据
        ret = addRealtimeTemperature(&temperature);
        if (ret < 0)  { puts("添加温度采集数据出错"); }

        //  更新统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        //  更新 最小温度/最大温度 记录值
        //  第一次设置统计
        if ((statistics.min_temperature==0) && (statistics.max_temperature==0))
        {
            statistics.warehouse       = WAREHOUSE_ID;
            statistics.min_temperature = temperature.value;
            statistics.max_temperature = temperature.value;
            strcpy(statistics.date, date);
        } 
        //  非第一次更新
        else 
        {
            if (temperature.value < statistics.min_temperature)
                statistics.min_temperature = temperature.value;
            if (temperature.value > statistics.max_temperature)
                statistics.max_temperature = temperature.value;
        }
        //  更新 温度异常计数, 本次温度存在异常, 并且上次采集不存在异常, 计数+1
        if (temperature.status && !lastTimeValue.status)
            statistics.abnormal_temperature += 1;
        ret = modifyStatistics(&statistics);
        if (ret < 0)  { puts("更新统计信息出错"); }
        
        //  记录本次的温度值为上次的温度值
        lastTimeValue = temperature;
    }

    return NULL;
}
//  湿度采集任务
void *gatherHumidityTask(void *arg) 
{
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送湿度采集命令
        //************************
        //  这里使用随机数模拟采集的湿度: -0.0 ~ 70.0 
        CommonValueModel humidity = {0};
        humidity.value = ((float)(rand() % 700)) / 10; 
        
        int ret = 0;
        //  查询湿度配置, 检查湿度是否异常
        ParamConfigModel config;
        int rows = queryParamConfig(TypeHumidity, &config);
        if (rows == 1)
        {
            //  湿度与最低检测湿度比较
            if (humidity.value < config.min) 
            {
                humidity.status = (int)(humidity.value - config.min);
                strcpy(humidity.message, "湿度过低");
            } 
            //  湿度与最高检测湿度比较
            else if (humidity.value > config.max)
            {
                humidity.status = (int)(humidity.value - config.max);
                strcpy(humidity.message, "湿度过高");
                
                /**
                //  湿度过高, 根据自动控制配置决定是否打开风扇
                if (config.automation)
                {
                    //  查询风扇现在的状态
                    DeviceStatusModel device;
                    rows = queryDeviceStatus(1, TypeFan, &device);
                    if (rows == 1)
                    {
                        //  风扇关闭状态, 打开风扇, 模式变为自动控制
                        if (device.status == 0)
                        {
                            //  先默认开启2档, 后期也可以根据湿度自动变化
                            device.status = 2;  
                            device.mode   = TypeAutomatic; 
                            //  更新设备状态
                            ret = modifyDeviceStatus(&device);
                            if (ret!=0) { puts("更新风扇状态失败"); }
                        }
                    }
                }
                **/
            }
        }
        //  添加湿度采集数据
        ret = addRealtimeHumidity(&humidity);
        if (ret < 0)  { puts("添加湿度采集数据出错"); }

        //  更新统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        //  更新 最小湿度/最大湿度 记录值
        //  第一次设置统计
        if ((statistics.min_humidity==0) && (statistics.max_humidity==0))
        {
            statistics.warehouse       = WAREHOUSE_ID;
            statistics.min_humidity = humidity.value;
            statistics.max_humidity = humidity.value;
            strcpy(statistics.date, date);
        } 
        //  非第一次更新
        else 
        {
            if (humidity.value < statistics.min_humidity)
                statistics.min_humidity = humidity.value;
            if (humidity.value > statistics.max_humidity)
                statistics.max_humidity = humidity.value;
        }
        //  更新 湿度异常计数, 本次湿度存在异常, 并且上次采集不存在异常, 计数+1
        if (humidity.status && !lastTimeValue.status)
            statistics.abnormal_humidity += 1;
        ret = modifyStatistics(&statistics);
        if (ret < 0)  { puts("更新统计信息出错"); }
        
        //  记录本次的湿度值为上次的湿度值
        lastTimeValue = humidity;
    }

    return NULL;
}
//  光照采集任务
void *gatherIlluminationTask(void *arg)
{
    CommonValueModel lastTimeValue = {0};
    while(1)
    {
        sleep(GATHER_INTERVAL);

        //************************
        //  这里向M0发送光照采集命令
        //************************
        //  这里使用随机数模拟采集的光照: -0.0 ~ 100.0 
        CommonValueModel illumination = {0};
        illumination.value = ((float)(rand() % 1000)) / 10; 
        
        int ret = 0;
        //  查询光照配置, 检查光照是否异常
        ParamConfigModel config;
        int rows = queryParamConfig(TypeIllumination, &config);
        if (rows == 1)
        {
            //  光照与最低检测光照比较
            if (illumination.value < config.min) 
            {
                illumination.status = (int)(illumination.value - config.min);
                strcpy(illumination.message, "光照过低");
                /**
                //  根据自动控制配置决定是否打开照明灯
                **/
            } 
            //  光照与最高检测光照比较
            else if (illumination.value > config.max)
            {
                illumination.status = (int)(illumination.value - config.max);
                strcpy(illumination.message, "光照过高");
            }
        }
        //  添加光照采集数据
        ret = addRealtimeIllumination(&illumination);
        if (ret < 0)  { puts("添加光照采集数据出错"); }

        //  更新统计数据 异常/最大/最小 值
        char date[20] = "";
        timestampToDatestr(time(NULL), date);
        StatisticsModel statistics = {0};
        rows = queryStatistics(WAREHOUSE_ID, date, &statistics);
        //  更新 最小光照/最大光照 记录值
        //  第一次设置统计
        if ((statistics.min_illuminance==0) && (statistics.max_illuminance==0))
        {
            statistics.warehouse       = WAREHOUSE_ID;
            statistics.min_illuminance = illumination.value;
            statistics.max_illuminance = illumination.value;
            strcpy(statistics.date, date);
        } 
        //  非第一次更新
        else 
        {
            if (illumination.value < statistics.min_illuminance)
                statistics.min_illuminance = illumination.value;
            if (illumination.value > statistics.max_illuminance)
                statistics.max_illuminance = illumination.value;
        }
        //  更新 光照异常计数, 本次光照存在异常, 并且上次采集不存在异常, 计数+1
        if (illumination.status && !lastTimeValue.status)
            statistics.abnormal_illuminance += 1;
        ret = modifyStatistics(&statistics);
        if (ret < 0)  { puts("更新统计信息出错"); }
        
        //  记录本次的光照值为上次的光照值
        lastTimeValue = illumination;
    }

    return NULL;
}
//  蜂鸣器控制任务
void *buzzerControlTask(void *arg)
{

}
//  照明灯控制任务
void *lightControlTask(void *arg)
{

}
//  风扇控制任务
void *fanControlTask(void *arg)
{

}













