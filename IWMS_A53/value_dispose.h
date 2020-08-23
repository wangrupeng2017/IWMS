/************************************************
 * Name: value_dispose.h
 * Date: 2020-08-23
 * Description: 负责 (温度/湿度/光照)等信息值的处理
 *              负责 (蜂鸣器,照明灯,风扇)等状态的管理
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-23
 ************************************************/
#include "store_models.h"

//  处理采集数状态
int disposeValueStatus(CommonValueModel *model, float min, float max);
//  处理统计值数据
int disposeStatisticsValue(CommonValueModel *model, int *abnormal, float *min, float *max);

