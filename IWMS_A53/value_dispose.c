/************************************************
 * Name: value_dispose.c
 * Date: 2020-08-23
 * Description: 负责 (温度/湿度/光照)等信息值的处理
 *              负责 (蜂鸣器,照明灯,风扇)等状态的管理
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-23
 ************************************************/
#include <string.h>
#include "value_dispose.h"


/***************************************
 * function:    处理采集数状态
 * @param [ in] model 当前采集值
 * @param [ in] min 统计最小值
 * @param [ in] max 统计最大值
 * @return      0:接受成功，!0:接受错误
 */
int disposeValueStatus(CommonValueModel *model, float min, float max)
{
    if (model->value < min)  
    {
        model->status = model->value - min;
        strcat(model->message, "过低");
    } 
    else if (model->value > max)  
    {
        model->status = model->value - max;
        strcat(model->message, "过高");
    }
    else
    {
        model->status = 0;
        strcat(model->message, "正常");
    }
    return 0;
}

/***************************************
 * function:    处理统计值数据
 * @param [ in] model 当前采集值
 * @param [out] abnormal 统计异常次数
 * @param [out] min 统计最小值
 * @param [out] max 统计最大值
 * @return      0:接受成功，!0:接受错误
 */
int disposeStatisticsValue(CommonValueModel *model, int *abnormal, float *min, float *max)
{
    if ((*min == 0) && (*max == 0))    *min = *max = model->value;
    if (model->value < *min)  *min = model->value;
    if (model->value > *max)  *max = model->value;
    if (model->status != 0)   *abnormal += 1;
    return 0;
}
