#include "Direction.h"

float state;//传感器融合
int32_t leftSensor, rightSensor, dirAngleSpeed;
#if defined(OUT_JUDGE) || defined(RS_JUDGE)
bool out;
#endif
#ifdef DYNAMIC_DC_PID
float DC_PID_P;
#endif

/**
 * @brief  方向PID准闭环, 有动态/静态P参数两种控制模式, 由DYNAMIC_DC_PID宏决定, 其于头文件Param.h中定义
 * @param[in]  leftSensor 左电感采集
 * \param[in]  middleSensor 中电感采集(可选)
 * \param[in]  rightSensor 右电感采集
 * \param[in]  dirAngleSpeed 方向陀螺仪采集(可选)
 * \param[in]  DC_PID_P 动态P, 经过计算得到(可选)
 * @retval 方向环输出, 作为标准电机输出的一环
 */
static int32_t Direction_PID(int32_t leftSensor, int32_t rightSensor, int32_t dirAngleSpeed
                             #ifdef DYNAMIC_DC_PID
                             , float DC_PID_P
                             #endif
                             )
{
    int32_t P, D;
    int32_t incpid;
    
    state = (float)(leftSensor - rightSensor) / (leftSensor + rightSensor);
    
    P = DC_PID_P * state;
    D = DC_PID_D * dirAngleSpeed;
    
    incpid = P + D;
    
    return incpid;
}

/**
 * @brief  标准方向环处理, 根据周期平滑输出, 周期在头文件Param.h中定义为DC_PERIOD
 * @param[in]  speed 由编码器采集的速度, 用于计算动态P
 * @retval 方向环输出, 作为标准电机输出的一环
 */
int32_t Direction_Process(int32_t speed)
{
    static uint8_t count = 0;
    static int32_t DC_Out_Old = 0, DC_Out_New = 0;
    int32_t DC_Out;
    
    if( count >= DC_PERIOD )
    {
        count = 0;
        DC_Out_Old = DC_Out_New;
//        Sensor_Get(&leftSensor, &rightSensor);
        
        #ifdef OUT_JUDGE
            if( leftSensor < OUT_JUDGE_STD && rightSensor < OUT_JUDGE_STD )
            {
                out = true;
            }
        #endif /* OUT_JUDGE */
                
        #ifdef DYNAMIC_DC_PID
            DC_PID_P = DC_PID_P_COEF * speed;
            if( DC_PID_P > DC_P_MAX )
            {
                DC_PID_P = DC_P_MAX;
            }
            else if( DC_PID_P < DC_P_MIN )
            {
                DC_PID_P = DC_P_MIN;
            }
        #endif
                
        DC_Out_New = Direction_PID(leftSensor, rightSensor, dirAngleSpeed
                                   #ifdef DYNAMIC_DC_PID
                                   , DC_PID_P
                                   #endif
                                   );
    }
    count++;
    
    DC_Out = DC_Out_Old + (DC_Out_New - DC_Out_Old) * count / DC_PERIOD;
    return DC_Out;
}
