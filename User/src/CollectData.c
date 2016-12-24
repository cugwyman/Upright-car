#include "CollectData.h"
#include "adc.h"

static int32_t Gyro_Zero;//���������
static int32_t DirGyro_Zero;//�������������
    //#if SENSOR_FLTR_SAMPLE > 1
    //static int32_t leftSensorBuffer[SENSOR_FLTR_SAMPLE];
    //static int32_t rightSensorBuffer[SENSOR_FLTR_SAMPLE];
    //#endif
    
    ///**
    // * @brief  �����������˲�, ����ƽ���˲�
    // * @param[in]  sensorBuffer ����������������
    // * \param[in]  newSample �²���ֵ
    // * \param[in]  sensor ȡ���˲������ֵ��ָ��
    // */
    //void Sensor_Filter(int32_t sensorBuffer[], int32_t newSample, int32_t* sensor)
    //{
    //	int i;
    //	for(i = 0; i < SENSOR_FLTR_SAMPLE - 1; i++)
    //	{
    //		sensorBuffer[i] = sensorBuffer[i+1];
    //	}
    //	sensorBuffer[SENSOR_FLTR_SAMPLE - 1] = newSample;
    //	*sensor = sensorBuffer[0];
    //	for(i = 1; i < SENSOR_FLTR_SAMPLE; i++)
    //	{
    //		*sensor += sensorBuffer[i];
    //		*sensor /= 2;
    //	}
    //}

/**
 * @brief  �ɼ�ģ���ʼ��, ������С������Ǻͼ��ٶȼ�, Ȩ�����׺�ѡ�������е�12or13λ����, MAP��ͷ�ļ�PinMap.h�ж���
 */
void Collection_Init(void)
{
    int32_t i;
//    ADC_QuickInit(LEFT_SENSOR_MAP,  kADC_SingleDiff12or13);
//    ADC_QuickInit(RIGHT_SENSOR_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(GYRO_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(DIR_GYRO_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(ACCE_MMAX_MAP, kADC_SingleDiff12or13);
    ADC_QuickInit(ACCE_MMAZ_MAP, kADC_SingleDiff12or13);
	Gyro_Zero = ADC_QuickReadValue(GYRO_MAP);
	DirGyro_Zero = ADC_QuickReadValue(DIR_GYRO_MAP);
	DelayMs(1);
    for(i = 0; i < GYRO_ZERO_SAMPLE; ++i)
    {
        Gyro_Zero += ADC_QuickReadValue(GYRO_MAP);
        DirGyro_Zero += ADC_QuickReadValue(DIR_GYRO_MAP);
		Gyro_Zero /= 2;
		DirGyro_Zero /= 2;
		DelayMs(1);
    }
}

///**
// * @brief  ��вɼ�, ���ڷ��򻷿���
// * @param[in]  leftSensor   ���вɼ�, ��ָ��ȡ��
// * \param[in]  rightSensor  �ҵ�вɼ�, ��ָ��ȡ��
// */
//void Sensor_Get(int32_t* leftSensor, int32_t* rightSensor)
//{
//	#if SENSOR_FLTR_SAMPLE > 1
//		Sensor_Filter(leftSensorBuffer,  ADC_QuickReadValue(LEFT_SENSOR_MAP),  leftSensor);
//		Sensor_Filter(rightSensorBuffer, ADC_QuickReadValue(RIGHT_SENSOR_MAP), rightSensor);
//	#else
//		*leftSensor = ADC_QuickReadValue(LEFT_SENSOR_MAP);
//		*rightSensor = ADC_QuickReadValue(RIGHT_SENSOR_MAP);
//	#endif
//}

/**
 * @brief  �����ǲɼ�, ����ֱ��������(�辭���˲�)
 * @retval �����ǲɼ�(��ȥ���)
 */
int32_t Gyro_Get(void)
{
	return ADC_QuickReadValue(GYRO_MAP) - Gyro_Zero;
}

/**
 * @brief  ���������ǲɼ�, ���ڷ��򻷿���
 * @retval ���������ǲɼ�(��ȥ���)
 */
int32_t DirGyro_Get(void)
{
    return ADC_QuickReadValue(DIR_GYRO_MAP) - DirGyro_Zero;
}

/**
 * @brief  ���ٶȼƲɼ�, ����ֱ��������(�辭���˲�)
 * @param[in]  mmax ���ٶȼ�X��ɼ�, ��ָ��ȡ��
 * \param[in]  mmaz ���ٶȼ�Z��ɼ�, ��ָ��ȡ��
 */
void Acce_Get(int32_t* mmax, int32_t* mmaz)
{
    *mmax = ADC_QuickReadValue(ACCE_MMAX_MAP) - MMAX_ZERO;
    *mmaz = ADC_QuickReadValue(ACCE_MMAZ_MAP) - MMAZ_ZERO;
}
