#include "TrackIdentify.h"
//mode MODE;

uint16_t MidLine[IMG_ROW];
uint16_t  LeftEdge[IMG_ROW],RightEdge[IMG_ROW];
 bool LeftFlag,RightFlag;
static bool LeftBorderSearchInRange(int16_t row, int16_t startIndex);
static bool RightBorderSearchInRange(int16_t row, int16_t startIndex);
 
//****************************************************************************
//  函数名：FittingLine_L( uint16_t n )
//  功能：最小二乘法拟合直线，并补齐左边界
//  说明：无
//*****************************************************************************/  
uint16_t FittingLine_L( uint16_t n )
{
  uint16_t i;
	float Lxy,xnum,ynum,xsquarenum,xaverage,yaverage;
	double a,b;
	for(i=n-4;i<=n-1;i++)
	{
		Lxy += i*LeftEdge[i];
		xnum +=LeftEdge[i];
    ynum +=i;
    xsquarenum += LeftEdge[i]*LeftEdge[i];		
	}
	xaverage = xnum/4;
	yaverage = ynum/4;
	a = (Lxy - xnum*ynum/4.0)/(xsquarenum - xnum*xnum/4.0);
	b = yaverage - a*xaverage;
	return (n-b)/a;
}

//****************************************************************************
//  函数名：FittingLine_R( uint16_t n )
//  功能：最小二乘法拟合直线，并补右边界
//  说明：无
//*****************************************************************************/  
uint16_t FittingLine_R( uint16_t n )
{
  uint16_t i;
	float Lxy,xnum,ynum,xsquarenum,xaverage,yaverage;
	double a,b;
	for(i=n-4;i<=n-1;i++)
	{
		Lxy += i*RightEdge[i];
		xnum +=RightEdge[i];
    ynum +=i;
    xsquarenum += RightEdge[i]*RightEdge[i];		
	}
	xaverage = xnum/4;
	yaverage = ynum/4;
	a = (Lxy - xnum*ynum/4.0)/(xsquarenum - xnum*xnum/4.0);
	b = yaverage - a*xaverage;
	return (n-b)/a;
}

//****************************************************************************
//  函数名：EdgeSearch( uint16_t n )
//  功能：检测赛道左右边线并计算出中线位置
//  说明：从图像中点开始同时往两边寻找边线，若找不到则补线
//*****************************************************************************/ 
 void EdgeSearch( uint16_t n ) {
	LeftEdge[n]=0;
	RightEdge[n]=0;
	MidLine[n]=0;
	uint16_t i,j,k;

	i=IMG_MIDPOINT;           //search from the midpoint
	for(j=i,k=i;(j<=i+108) && (k>=i-108);j++,k--)
	{
//		if((imgBuf[n][j]&imgBuf[n][j-1]&imgBuf[n][j-2]&imgBuf[n][j-3]==1)&&(imgBuf[n][j+1]|imgBuf[n][j+2]|imgBuf[n][j+3]|imgBuf[n][j+4]==0))
		if(imgBuf[n][j] && !imgBuf[n][j+1] )
		{
			RightEdge[n]=j+1;
//			printf("%d\n",RightEdge[n]);
		}
//		if((imgBuf[n][k]|imgBuf[n][k-1]|imgBuf[n][k-2]|imgBuf[n][k-3]==0)&&(imgBuf[n][k+1])&imgBuf[n][k+2]&imgBuf[n][k+3]&imgBuf[n][k+4]==1)
		if(!imgBuf[n][k] && imgBuf[n][k+1] )
		{
			LeftEdge[n]=k;
		}
	}
		/*左右边界都找到*/
//    if(RightEdge[n] && LeftEdge[n])
//		{
//			MidLine[n]=(RightEdge[n]+LeftEdge[n])/2;
//			break;
//		}
//		/*只找到左边界，则根据最小二乘法拟合直线补齐右边界*/
//		if(!RightEdge[n] && LeftEdge[n])
//		{
//			RightEdge[n]=FittingLine_R(n);
//			break;
//////			printf("right");
//////			DelayMs(1000);
//		}
////		/*只找到右边界，则根据最小二乘法拟合直线补齐左边界*/
//		else if(RightEdge[n] && !LeftEdge[n])
//		{
//			LeftEdge[n]=FittingLine_L(n);
//			break;
////			printf("left");
//		}
////		/*左右边界都没找到，则补齐左右边界*/
      if(!RightEdge[n] && !LeftEdge[n])
		{
			RightEdge[n]=RightEdge[n-1];
			LeftEdge[n]=LeftEdge[n-1];
		}
	
}


void GetMidLine( uint16_t n ) {
	MidLine[n] = (LeftEdge[n]+RightEdge[n])/2;
}


void SensorGet(float* farimg, float* nearimg)
{
	uint16_t i;
	float nearsum,farsum;
	for(i=MODE.foresight-4;i<MODE.foresight+4;i++)
	{
		nearsum+=MidLine[i];
	}
	*nearimg = nearsum/8;
	for(i=21;i<IMG_ROW;i++)
	{
		farsum+=MidLine[i];
	}
	*farimg = farsum/30;
}


bool LeftBorderSearch(int16_t row) {
    if(row == 0) { //first line
        return LeftBorderSearchInRange(row, IMG_COL / 2 + IMG_BORDER_SCAN_COMPENSATION);
    } else {
           if(RightEdge[row - 1] - LeftEdge[row - 1] <= WIDE_ROAD_SIZE) {
            return LeftBorderSearchInRange(row, MidLine[row - 1]);
        } else {
            return LeftBorderSearchInRange(row, LeftEdge[row - 1] + IMG_BORDER_SCAN_COMPENSATION);
        }
			}
}

bool LeftBorderSearchInRange(int16_t row, int16_t startIndex) {
    for(int16_t j = startIndex; j >= 0; --j) {
        if(!imgBuf[row][j] && imgBuf[row][j+1]) {
            LeftEdge[row] = j;
            return true;
        }
    }
    LeftEdge[row] = 0;
    return false;
}

bool RightBorderSearch(int16_t row) {
    if(row == 0) { //first line
        return RightBorderSearchInRange(row, IMG_COL / 2 - IMG_BORDER_SCAN_COMPENSATION);
    } else {
        if(RightEdge[row - 1] - LeftEdge[row - 1] <= WIDE_ROAD_SIZE) {
            return RightBorderSearchInRange(row, MidLine[row - 1]);
        } else {
            return RightBorderSearchInRange(row, RightEdge[row - 1] - IMG_BORDER_SCAN_COMPENSATION);
        }
    }
}

bool RightBorderSearchInRange(int16_t row, int16_t startIndex) {
    for(int16_t j = startIndex; j < IMG_COL - 1; ++j) {
        if(imgBuf[row][j] && !imgBuf[row][j+1]) {
            RightEdge[row] = j;
            return true;       //return后便不再执行
        }
    }
    RightEdge[row] = IMG_COL - 1;
    return false;
}


void MiddleLineUpdate(int16_t row) {
    if(imgProcFlag == CIRCLE)
	{
		MidLine[row] = RightEdge[row] - 80;
	}
	else
	{
    MidLine[row] = (LeftEdge[row] + RightEdge[row]) / 2;
	}
	
}

//		i=LeftEdge[n-1];     //search from the last edge position
//		for(j=i-40;j<=i+40;j++)
//		{
//			if((!imgBuf[n][j])&&(imgBuf[n][j+1]))
//		{
//			LeftEdge[n]=j;
//		}
//		}
//		i=RightEdge[n-1];
//		for(j=i-40;j<=i+40;j++)
//		{
//			if((imgBuf[n][j])&&(!imgBuf[n][j+1]))
//		{
//			RightEdge[n]=j+1;
//			break;
//		}
//		}	MidLine[n]=(RightEdge[n]+LeftEdge[n])/2;
//	}



/****************************************************************************
  函数名：CurveCal(uint16_t n)
  功能：初步判断赛道为弯道还是直道
  说明：计算本场中线位置与平均值的偏差，若偏差较小则为直道，否则为曲线，继续进行后续判断
*****************************************************************************/ 
 uint16_t CurveCal(uint16_t n) {
	 float sum,curve;
   uint16_t i,RoadFlag=0;
	 for(i=0;i<n;i++) {
		 sum+=MidLine[i];
	 }
	 for(i=0;i<n;i++) {
		 curve+=(MidLine[i]-sum/n)*(MidLine[i]-sum/n);
//		 if (MidLine[i]-sum/n > 0)
//		 curve+=MidLine[i]-sum/n;
//		 else curve-=MidLine[i]-sum/n;
	 }
	 curve=curve/n;
	 if (curve < 100 ) {
   		 RoadFlag = STRAIGHTROAD;
	 }
	 else {
		   RoadFlag = CURVEROAD;
	 }
	 		 return RoadFlag;
 }
 
////****************************************************************************
////  函数名：SlopeCal( uint16_t n)
////  功能：判断赛道为普通弯道还是大S弯道 ，小S与普通弯道视作同样情况处理
////  说明：利用远中近三点的连线斜率判断弯道类型
////*****************************************************************************/ 
// uint16_t SlopeCal( uint16_t n) {
//	 int16_t Slope_a,Slope_b;
//	 int16_t CurveTypeFlag;
//	 if ( n > 4 ) {
//		 Slope_a = (MidLine[n/2] - MidLine[0])/n*2;
//		 Slope_b = (MidLine[n] -  MidLine[n/2])/n*2;
//	 }
//	 else {
//		 Slope_a = 0;
//		 Slope_b = 0;
//	 }
//	 if (Slope_a*Slope_b > 0 && Slope_a > 0 ) {
//		 CurveTypeFlag = LEFTCURVE;
//	 }
//	 else if (Slope_a*Slope_b > 0 && Slope_a < 0 ) {
//		 CurveTypeFlag = RIGHTCURVE;
//	 }
//	 else if (Slope_a*Slope_b < 0 && Slope_a > 0 ) {
//		 CurveTypeFlag = NORMALS;
//	 }
//	 else if (Slope_a*Slope_b < 0 && Slope_a < 0 ) {
//		 CurveTypeFlag = OPPOSITES;
//		}
//	 return CurveTypeFlag;
// }
// 
////****************************************************************************
////  函数名：BlockRec(uint16_t n)
////  功能：判断本场赛道为十字还是障碍
////  说明：中线位置计算为0时判断为十字，否则判断是否为障碍物
////*****************************************************************************/ 
// uint16_t BlockRec(uint16_t n) {
//	 uint16_t BlockFlag;
//	 uint16_t i;
//	 for(i=0;i<n;i++) {
//	 if (MidLine[i])
//	 {
//	 if (MidLine[i]-112 >= 38 ) 
//	 {
//		 BlockFlag = LEFTBLOCK ;
//	 }
//	 if (MidLine[i]-112 <= -38 )
//	 {
//		 BlockFlag = RIGHTBLOCK ;
//	 }
//   }
//	 else BlockFlag = CROSSROAD ;
//  }
//	 return BlockFlag;
// }


 uint16_t CircleRec(uint16_t n) {
	 uint16_t CircleFlag;
	 //Do something
	 return CircleFlag;
 }
