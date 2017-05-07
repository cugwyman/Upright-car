//#include "ftmQD.h"
//#include "VisualScope.h"
//#define VAL_P 150
//#define VAL_I 0.5
//#define VAL_D 2

////#define VAL_P 300
////#define VAL_I 0.1
////#define VAL_D 30

//int16_t Speed_Now_S;
//long Speed_Fix_M;
//int16_t Speed_Now_B;

////int error;



///* 转速PID */
//int16_t VAL_PID(int16_t Speed_Set,int16_t Speed_Now)
//{
//	static int16_t Pvalue,Ivalue, Dvalue;
//    static int16_t error=0;
//    static int16_t lastError = 0;
//    static int16_t prevError = 0;
//	static int16_t Speed_Fix = 0;

//	error = Speed_Set - Speed_Now;
//	Pvalue = ( error - lastError ) * VAL_P ;
//	Ivalue = error * VAL_I;
//	Dvalue = ( error - 2 * lastError + prevError ) * VAL_D;
//	
//	prevError = lastError;
//	lastError = error;
//	 
//	Speed_Fix += Pvalue + Ivalue + Dvalue;
////	Speed_Fix += (( error - lastError ) * VAL_P + error * VAL_I + ( error - 2 * lastError + prevError ) * VAL_D);

//	if(Speed_Fix>10000)
//    Speed_Fix = 10000;
//  if(Speed_Fix < -10000)
//		Speed_Fix = -10000;

//	return Speed_Fix;
//}


////int16_t VAL_PID(int16_t speed_set,int16_t speed)
////{
////static int16_t error,last_error,last_error_1;
////static int16_t PID_Result_L;
////error = speed_set - speed;
////PID_Result_L += VAL_P * (error - last_error) + VAL_I * error +VAL_D * (error - 2 * last_error + last_error_1 );
////last_error_1 = last_error;
////last_error = error;
////if(PID_Result_L>10000)
////PID_Result_L = 10000;
////if(PID_Result_L < -10000)
////PID_Result_L = -10000;
////return PID_Result_L;
////}


////long VAL_PID(int32_t speed_set,int32_t speed)
////{
////static int last_error,last_error_1;
////static long PID_Result_L;
////error = speed_set - speed;
////PID_Result_L += VAL_P * (error - last_error) + VAL_I * error +VAL_D * (error - 2 * last_error + last_error_1 );
////last_error_1 = last_error;
////last_error = error;
////if(PID_Result_L>10000)
////PID_Result_L = 10000;
////if(PID_Result_L < -10000)
////PID_Result_L = -10000;
////return PID_Result_L;
////}


//int16_t Speed_Filter_L(int16_t v)    //正交解码后滤波：4数去首尾，均值滤波
//{
//    int m,n,t;
//    int16_t num1[4];
//    static int16_t num2[4];
//    num2[0]=num2[1];
//    num2[1]=num2[2];
//    num2[2]=num2[3];
//    num2[3]=v;
//    for(m = 0;m<4;m++)
//        num1[m] = num2[m];
//    for (m = 0;m < 3;m++)
//        for(n = m+1;n < 4;n++)
//        {
//          if (num1[m] < num1[n])
//          {
//            t = num1[n];
//            num1[n] = num1[m];
//            num1[m] = t;
//          }
//        }
//    return (num1[1] + num1[2] )/2;
//}

///*使用FTM1模块的PTA8、PTA9引脚进行正交解码
//      通过串口将正交解码的数据发送出去
//			
//			PIT模块：固定间隔时间内获取计数值才能算出频率
//*/
//void FtmQD_Init(void)  //正交解码初始化
//{
//	  FTM_QD_QuickInit(FTM1_QD_PHA_PA08_PHB_PA09, kFTM_QD_NormalPolarity, kQD_CountDirectionEncoding);

//}

//int16_t FtmQD_Get(void)//正交解码引用函数
//{
//    int16_t value; /* 记录正交脉冲个数 */
//    uint8_t dir; /* 记录编码器旋转方向1 */
//    /* 获取正交解码数据 */
//    FTM_QD_GetData(HW_FTM1, &value, &dir);
////    printf("value:%6d dir:%d  \r", value, dir);
//    FTM_QD_ClearCount(HW_FTM1); /* 如测量频率则需要定时清除Count值  */
//    return value;
//}

////void PIT_ISR0(void)
////{
////	  Speed_Now_S=FtmQD_Get();
////}

//void PIT_ISR0(void)     //中断函数
//{
//    static int16_t a=0; //每四个值进行一次正交解码
//    a++;

//	  Speed_Now_B=FtmQD_Get();//正交解码数据
//	  Speed_Now_S=Speed_Filter_L( Speed_Now_B);//滤波后
//      if(a>2)
//	  {
//			Speed_Fix_M=VAL_PID(200,Speed_Now_S);//PID返回修正值
//			a=0;              //选值要清零
//	  }

////	if(Speed_Fix_M < 0)         //跳帽直接使能，一路PWM波一路接地
////    {
////        Speed_Fix_M = -Speed_Fix_M;
////    }

//	   if(Speed_Fix_M >= 0)     //跳帽PWM波，一路接0一路接1
//        {
//		  GPIO_WriteBit(HW_GPIOC, 2, 0);
//          GPIO_WriteBit(HW_GPIOC, 3, 1);

//		}
//		else
//		{
//		 GPIO_WriteBit(HW_GPIOC, 2, 1);
//         GPIO_WriteBit(HW_GPIOC, 3, 0);
//         Speed_Fix_M = -Speed_Fix_M;
//		}
//      Motor_Set(Speed_Fix_M);
////  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 4000); /* 0-10000 对应 0-100% */   测试用

//	  VisualScope_Output( 200 , Speed_Now_B , Speed_Now_S , Speed_Fix_M);
// 
//}


//void _PIT_Init(void)
//{

//    PIT_QuickInit(HW_PIT_CH0,1000*5);//1000=1ms
//    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR0);
//    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, ENABLE);


//}

