#include "TrackIdentify.h"
//mode MODE;

uint16_t MidLine[IMG_ROW];
uint16_t avgMidLine;
uint16_t  LeftEdge[IMG_ROW],RightEdge[IMG_ROW];
float leftSlope[IMG_ROW];
float leftZero[IMG_ROW];
float rightSlope[IMG_ROW];
float rightZero[IMG_ROW];
float middleSlope[IMG_ROW];
float middleZero[IMG_ROW];
int16_t rightBorderNotFoundCnt;
int16_t leftBorderNotFoundCnt;
bool LeftFlag,RightFlag;
bool LeftBorderSearchInRange(int16_t row, int16_t startIndex);
bool RightBorderSearchInRange(int16_t row, int16_t startIndex);
static bool IsCurve(void);
static bool IsCrossRoad(void);
static inline float Abs(float);
static inline float Min(float, float);
static inline float Max(float ,float);
 
//*****************************************************************************/
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

//*****************************************************************************/
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
//*****************************************************************************/
//  函数名：FittingMidLine( void )
//  功能：最小二乘法拟合直线，补中线
//  说明：无
//*****************************************************************************/  
void FittingMidLine( void )
{
	uint16_t i;
	float Lxy,xnum,ynum,xsquarenum,xaverage,yaverage;
	double a,b;
	for(i=0;i<IMG_ROW;i++)
	{
		Lxy += i*MidLine[i];
		xnum +=MidLine[i];
    ynum +=i;
    xsquarenum += MidLine[i]*MidLine[i];		
	}
	xaverage = xnum/50;
	yaverage = ynum/50;
	a = (Lxy - xnum*ynum/50)/(xsquarenum - xnum*xnum/50);
	b = yaverage - a*xaverage;
	for(i=0;i<IMG_ROW;i++)
	{
		MidLine[i] = (i-b)/a;
	}
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

void CurveSlopeUpdate(int16_t row) {
    if(row >= 4) {
        float leftSlopeX = 0, leftSlopeA = 0, leftSlopeB = 0;
        float rightSlopeX = 0, rightSlopeA = 0, rightSlopeB = 0;
        float middleSlopeX = 0, middleSlopeA = 0, middleSlopeB = 0;
        for(int16_t i = row - 4; i <= row; ++i) {
            leftSlopeX += LeftEdge[i];
            leftSlopeA += i * LeftEdge[i];
            rightSlopeX += RightEdge[i];
            rightSlopeA += i * RightEdge[i];
            middleSlopeX += MidLine[i];
            middleSlopeA += i * MidLine[i];
        }
        leftSlopeB = (row - 2) * leftSlopeX;
        rightSlopeB = (row - 2) * rightSlopeX;
        middleSlopeB = (row - 2) * middleSlopeX;
        
        leftSlope[row] = (leftSlopeA - leftSlopeB) / 10.0;
        leftZero[row] = (leftSlopeX / 5) - leftSlope[row] * (row - 2);
        rightSlope[row] = (rightSlopeA - rightSlopeB) / 10.0;
        rightZero[row] = (rightSlopeX / 5) - rightSlope[row] * (row - 2);
        middleSlope[row] = (middleSlopeA - middleSlopeB) / 10.0;
        middleZero[row] = (middleSlopeX / 5) - middleSlope[row] * (row - 2);
    } else {
        leftSlope[row] = 0;
        leftZero[row] = 0;
        rightSlope[row] = 0;
        rightZero[row] = IMG_COL - 1;
        middleSlope[row] = 0;
        middleZero[row] = IMG_COL / 2;
    }
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


bool LeftBorderSearch(int16_t row) 
{
    if(row == 0)  //first line
        return LeftBorderSearchInRange(row, IMG_COL / 2 + IMG_BORDER_SCAN_COMPENSATION);
    else 
        {
            if(RightEdge[row - 1] - LeftEdge[row - 1] <= WIDE_ROAD_SIZE) 
                return LeftBorderSearchInRange(row, MidLine[row - 1]);
            else 
                return LeftBorderSearchInRange(row, LeftEdge[row - 1] + IMG_BORDER_SCAN_COMPENSATION);
		}
}

bool LeftBorderSearchInRange(int16_t row, int16_t startIndex) 
{
    for(int16_t j = startIndex; j >= 0; --j) 
    {
        if(imgBuf[row][j] && !imgBuf[row][j+1]) 
            {
                LeftEdge[row] = j;
                return true;
            }
    }
    LeftEdge[row] = 0;
    ++leftBorderNotFoundCnt;
//  printf("%d\n",leftBorderNotFoundCnt);
    return false;
}

bool RightBorderSearch(int16_t row) 
{
    if(row == 0)  //first line
        return RightBorderSearchInRange(row, IMG_COL / 2 - IMG_BORDER_SCAN_COMPENSATION);
    else 
        {
            if(RightEdge[row - 1] - LeftEdge[row - 1] <= WIDE_ROAD_SIZE) 
                return RightBorderSearchInRange(row, MidLine[row - 1]);
            else 
                return RightBorderSearchInRange(row, RightEdge[row - 1] - IMG_BORDER_SCAN_COMPENSATION);
        }
}

bool RightBorderSearchInRange(int16_t row, int16_t startIndex) 
{
    for(int16_t j = startIndex; j < IMG_COL - 1; ++j) 
        {
            if(!imgBuf[row][j] && imgBuf[row][j+1]) 
                {
                    RightEdge[row] = j;
                    return true;       //return后便不再执行
                }
        }
    RightEdge[row] = IMG_COL - 1;
    ++rightBorderNotFoundCnt;
//	printf("%d\n",rightBorderNotFoundCnt);
    return false;
}


void MiddleLineUpdate(int16_t row) {
    if(imgProcFlag == CIRCLE)
	{
		MidLine[row] = RightEdge[row] - 55;
	}
	else
	{
    MidLine[row] = (LeftEdge[row] + RightEdge[row]) / 2;
	}
	
}

void UpdateMiddleLine() {
    for(int row = 0; row < IMG_ROW; ++row) {
        MiddleLineUpdate(row);
    }
}

road_type_type GetRoadType() {
	if(IsCurve()) return Curve;
	else if(IsCrossRoad()) return CrossRoad;
	else return Unknown;
//    return IsCurve() ? Curve :CrossRoad
//        IsCrossRoad() ? CrossRoad :
//        Unknown;
}

bool IsCurve() {
    int blackCnt = 0;
    for (int row = IMG_ROW - 1; row >= 40; --row)
    {
        if (!imgBuf[row][MidLine[row]]) //中线所在点是否为黑色
        {
            ++blackCnt;
        }
    }
    return blackCnt > 5;
}

bool IsCrossRoad() {
//	printf("%d\n%d\n",leftBorderNotFoundCnt,rightBorderNotFoundCnt);
    return ((leftBorderNotFoundCnt > 3) && (rightBorderNotFoundCnt > 3)
        && ((leftBorderNotFoundCnt + rightBorderNotFoundCnt) > 10));
}

void CurveCompensate() {
    int row;
    for (row = IMG_ROW - 1; row > 8; --row)
    {
        if (!imgBuf[row][MidLine[row]])
        {
            break;
        }
    }
    if (leftBorderNotFoundCnt > 6 && rightBorderNotFoundCnt < 2)
    {
        for (int row_ = IMG_ROW - 1; row_ > row; --row_)
        {
            MidLine[row_] = 0;
        }
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            MidLine[row - cnt] = cnt * MidLine[row - 12] / 12;
        }
    }
    else if (rightBorderNotFoundCnt > 6 && leftBorderNotFoundCnt < 2)
    {
        for (int row_ = IMG_ROW - 1; row_ > row; --row_)
        {
            MidLine[row_] = IMG_COL - 1;
        }
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            MidLine[row - cnt] = IMG_COL - 1 - cnt * (IMG_COL - 1 - MidLine[row - 12]) / 12;
        }
    }
}

void CrossRoadCompensate() {
    int leftCompensateStart = IMG_ROW;
    int rightCompensateStart = IMG_ROW;
    int leftCompensateEnd = IMG_ROW;
    int rightCompensateEnd = IMG_ROW;

    {
        int row = 6;
        while ((row < IMG_ROW && LeftEdge[row] != 0)
            && (Abs(leftSlope[row] - leftSlope[row - 1]) < 3 )&& (leftSlope[row] * leftSlope[row - 1] >= 0)) { ++row; }
        leftCompensateStart = row;
    }

    {
        int row = 6;
        while ((row < IMG_ROW && RightEdge[row] != IMG_COL - 1)
            && (Abs(rightSlope[row] - rightSlope[row - 1]) < 3) && (leftSlope[row] * leftSlope[row - 1] >= 0)) { ++row; }
        rightCompensateStart = row;
    }

    for (int row = leftCompensateStart; row < leftCompensateEnd; ++row)
    {
        LeftEdge[row] = row * leftSlope[leftCompensateStart - 5] + leftZero[leftCompensateStart - 5];
        if(!imgBuf[row][LeftEdge[row]])
        {
            leftCompensateEnd = row;
            break;
        }
    }

    for (int row = rightCompensateStart; row < rightCompensateEnd; ++row)
    {
        RightEdge[row] = row * rightSlope[rightCompensateStart - 5] + rightZero[rightCompensateStart - 5];
        if (!imgBuf[row][RightEdge[row]])
        {
            rightCompensateEnd = row;
            break;
        }
    }

    int borderSearchStart;

    if (leftCompensateEnd < rightCompensateEnd)
    {
        borderSearchStart = (LeftEdge[leftCompensateEnd - 1] + RightEdge[leftCompensateEnd - 1]) / 2;
        for (int row = leftCompensateEnd; row < rightCompensateEnd; ++row)
        {
            LeftBorderSearchInRange(row, borderSearchStart);
            //if (Math.Abs((resultSet.rightBorder[row] + resultSet.leftBorder[row]) - (resultSet.rightBorder[row - 1] + resultSet.leftBorder[row - 1])) < 10)
            //{
                borderSearchStart = (RightEdge[row] + LeftEdge[row]) / 2;
            //}
        }
    }
    else if (leftCompensateEnd > rightCompensateEnd)
    {
        borderSearchStart = (LeftEdge[rightCompensateEnd - 1] + RightEdge[rightCompensateEnd - 1]) / 2;
        for (int row = rightCompensateEnd; row < leftCompensateEnd; ++row)
        {
            RightBorderSearchInRange(row, borderSearchStart);
            //if (Math.Abs((resultSet.rightBorder[row] + resultSet.leftBorder[row]) - (resultSet.rightBorder[row - 1] + resultSet.leftBorder[row - 1])) < 10)
            //{
                borderSearchStart = (RightEdge[row] + LeftEdge[row]) / 2;
            //}
        }
    }
    
    int compensateEnd = Max(leftCompensateEnd, rightCompensateEnd);

    borderSearchStart = (LeftEdge[compensateEnd - 1] + RightEdge[compensateEnd - 1]) / 2;
    for (int row = compensateEnd; row < IMG_ROW; ++row)
    {
        LeftBorderSearchInRange(row, borderSearchStart);
        RightBorderSearchInRange(row, borderSearchStart);
        if (Abs((RightEdge[row] + LeftEdge[row]) - (RightEdge[row - 1] + LeftEdge[row - 1])) < 10)
        {
            borderSearchStart = (RightEdge[row] + LeftEdge[row]) / 2;
        }
    }
    
    UpdateMiddleLine();
}

inline float Abs(float input) {
    return input >= 0 ? input : -input;
}

inline float Min(float a, float b) {
    return a > b ? b : a;
}

inline float Max(float a, float b) {
    return a > b ? a : b;
}


//Huge_Curve  judge and deal with 
void HugeCurveDeal(int16_t row){
        if(HugeCurve==-1) //left Huge_Curve  
         {
             MidLine[row]=0;    
         }
         if(HugeCurve==1) //right Huge_Curve
         {
             MidLine[row]=IMG_COL - 1;
         }  
         //  left Huge_Curve  judge and deel with      
         if((HugeCurve!=-1)&&(HugeCurve!=1)&&(row>15)&&(LeftEdge[row-5]==0)&&(LeftEdge[row-6]==0)&&(RightEdge[row-5]!=IMG_COL - 1)&&(RightEdge[row-6]!=IMG_COL - 1))
         {
             HugeCurve=-1;
             MidLine[row]=0;
         // middleline smooth 
             for(int i=row-6;i<=row;i++)
                {
                   MidLine[i]=MidLine[row-6]- MidLine[row-6]*(i-row+6)/8.0 ;
                }     
         }   
         //  right Huge_Curve  judge and deel with  
         if((HugeCurve!=-1)&&(HugeCurve!=1)&&(row>15)&&(RightEdge[row-5]==IMG_COL - 1)&&(RightEdge[row-6]==IMG_COL - 1)&&(LeftEdge[row-5]!=0)&&(LeftEdge[row-6]!=0))
         {
             HugeCurve=1;
             MidLine[row]=IMG_COL - 1;
         // middleline smooth 
              for(int i=row-6;i<=row;i++)
                 {
                   MidLine[i]= MidLine[row-6]+(IMG_COL - 1-MidLine[row-6])*(i-row+6)/8.0;
                 }
                      
         }
          if(HugeCurve==-1) //left Huge_Curve  
         {
             MidLine[row]=0;    
         }
         if(HugeCurve==1) //right Huge_Curve
         {
             MidLine[row]=IMG_COL - 1;
         }  
                 
}

bool StraightRoadJudge(void) {
 int16_t middleAreaCnt = 0;
    for(int16_t i = 0; i < IMG_ROW; ++i) {
        if( (IMG_COL / 2 - 10 < MidLine[i]) && (MidLine[i] < IMG_COL / 2 + 10)) {
            middleAreaCnt++;
        }
    }
    return middleAreaCnt > 38;
}

void ImgRevise(int16_t row) {
	int16_t x,y;
	float x1,y1;
	uint16_t i;
	float k1,k2;
	k1 = -0.0000026;
	k2 = -0.0000026;
	y = row - 25;
	for(i=0;i<IMG_ROW;i++) {
		x = i - 112;
		x1 = x*(1+k1*x*x+k2*y*y);
		y1 = y*(1+k1*x*x+k2*y*y);
		y1 = y1+25;
		x1 = x1+112;
		 if( x1 < 1 || x1 > 225 || y1 < 1 || y1 > 50)

                 img_undist[row][i] = 0;

             else

                 img_undist[row][i] = imgBuf[(uint16_t)y1][(uint16_t)x1];

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
// uint16_t CurveCal(uint16_t n) {
//	 float sum,curve;
//   uint16_t i,RoadFlag=0;
//	 for(i=0;i<n;i++) {
//		 sum+=MidLine[i];
//	 }
//	 for(i=0;i<n;i++) {
//		 curve+=(MidLine[i]-sum/n)*(MidLine[i]-sum/n);
////		 if (MidLine[i]-sum/n > 0)
////		 curve+=MidLine[i]-sum/n;
////		 else curve-=MidLine[i]-sum/n;
//	 }
//	 curve=curve/n;
//	 if (curve < 100 ) {
//   		 RoadFlag = STRAIGHTROAD;
//	 }
//	 else {
//		   RoadFlag = CURVEROAD;
//	 }
//	 		 return RoadFlag;
// }
 
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
