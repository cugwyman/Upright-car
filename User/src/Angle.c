#include "Angle.h"

float Kal_Gyro;
int32_t gyro;
int32_t mmax, mmaz;
int32_t angle, angleSpeed;
float Angle_Kalman;
float angleSpeedIntegral;

/**
 * @brief  卡尔曼滤波, 输入带有噪声的参数组, 返回可靠的参数组, 此处用于获得变化较稳定的角度值
 * @param[in]  angle_kal 由加速度计得到的角度参量
 * \param[in]  angle_speed_kal 由陀螺仪得到的角速度参量
 * @retval 稳定可靠的角度值, 用于直立控制
 */
static float Kalman_Filter(float angle_kal, float angle_speed_kal)
{
    static float angle_kalman = 0;
    static float Pk[2][2] = {{1, 0}, {0, 1}};
    float Pdot[4] ={0,0,0,0};
	static float Kal_Gyro_Zero = 0;
    //float gGyro;
    float angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
	angle_kalman += (angle_speed_kal - Kal_Gyro_Zero) * Kalman_Sample_Time;
	Pdot[0] = Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1] = -Pk[1][1];
	Pdot[2] = -Pk[1][1];
	Pdot[3] = Q_gyro;
	Pk[0][0] += Pdot[0] * Kalman_Sample_Time;
	Pk[0][1] += Pdot[1] * Kalman_Sample_Time;
	Pk[1][0] += Pdot[2] * Kalman_Sample_Time;
	Pk[1][1] += Pdot[3] * Kalman_Sample_Time;
	angle_err = angle_kal - angle_kalman;
	PCt_0 = C_0 * Pk[0][0];
	PCt_1 = C_0 * Pk[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * Pk[0][1];
	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	angle_kalman += K_0 * angle_err;
	Kal_Gyro_Zero += K_1 * angle_err;
	Kal_Gyro = angle_speed_kal - Kal_Gyro_Zero;
	return angle_kalman;
}

/**
 * @brief  角度PID准闭环
 * @param[in]  set 设定目标角度, 尽可能接近实际的机械零点, 在头文件Param.h中定义为AC_Set
 * \param[in]  nextPoint 当前角度值
 * @retval 角度环输出, 作为标准电机输出的一环
 */
static int32_t Angle_PID(float set, float nextPoint) 
{ 
    float error;
//    static float lastError;
    float P, D;
    int32_t incpid;
    
    error = set - nextPoint;
    P = AC_PID_P * error;
    D = AC_PID_D * angleSpeed;
    
//    lastError = error;
    
    incpid = P - D;
    
    return(incpid);
}

/**
 * @brief  标准角度环处理
 * @retval 角度环输出, 作为标准电机输出的一环
 */
int32_t Angle_Process(void)
{
    gyro = Gyro_Get();
    Acce_Get(&mmax, &mmaz);
    angle = (mmaz - mmax) * MMA_GAIN;
    angleSpeed = -gyro;
	angleSpeedIntegral += angleSpeed * 0.005;
    Angle_Kalman = Kalman_Filter(angle, angleSpeed);
    return -Angle_PID(AC_Set, Angle_Kalman);//暂且取相反数
}