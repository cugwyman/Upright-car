#include "CollectData.h"
#include "adc.h"

static int32_t Gyro_Zero;//陀螺仪零点
static int32_t DirGyro_Zero;//方向陀螺仪零点

/**
 * @brief  采集模块初始化, 包含电感、陀螺仪和加速度计, 权衡利弊后选择了适中的12or13位精度, MAP在头文件PinMap.h中定义
 */
void CollectInit(void) 
{
    int32_t i;
    ADC_QuickInit(GYRO_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(DIR_GYRO_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(ACCE_MMAX_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(ACCE_MMAZ_MAP, kADC_SingleDiff12or13);
	Gyro_Zero = ADC_QuickReadValue(GYRO_MAP);
	DirGyro_Zero = ADC_QuickReadValue(DIR_GYRO_MAP);
	DelayMs(1);
    for(i = 0; i < GYRO_ZERO_SAMPLE; ++i)//均值滤波，取1ms内均值
    {
        Gyro_Zero += ADC_QuickReadValue(GYRO_MAP);
        DirGyro_Zero += ADC_QuickReadValue(DIR_GYRO_MAP);
		Gyro_Zero /= 2;
		DirGyro_Zero /= 2;
		DelayMs(1);
    }
}


/**
 * @brief  陀螺仪采集, 用于直立环控制(需经过滤波)
 * @retval 陀螺仪采集(已去零点)
 */
int32_t GyroGet(void) 
{
	return ADC_QuickReadValue(GYRO_MAP) - Gyro_Zero;
}

/**
 * @brief  方向陀螺仪采集, 用于方向环控制
 * @retval 方向陀螺仪采集(已去零点)
 */
int32_t DirGyroGet(void) 
{
    return ADC_QuickReadValue(DIR_GYRO_MAP) - DirGyro_Zero;
}

/**
 * @brief  加速度计采集, 用于直立环控制(需经过滤波)
 * @param[in]  mmax 加速度计X轴采集, 由指针取走
 * \param[in]  mmaz 加速度计Z轴采集, 由指针取走
 */
void AcceGet(int32_t* mmax, int32_t* mmaz) 
{
    *mmax = ADC_QuickReadValue(ACCE_MMAX_MAP) - MMAX_ZERO;
    *mmaz = ADC_QuickReadValue(ACCE_MMAZ_MAP) - MMAZ_ZERO;
}
