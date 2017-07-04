#include "PatternMatch.h"
#include "ImgUtility.h"
#include "ImgProc.h"
#include "Motor.h"
#include "stdio.h"
#include "uart.h"
#include "gpio.h"
#include "Velocity.h"

int16_t ring_offset;
int16_t ringend_offset;
int32_t ringDistance;
int32_t crossRoadDistance;
bool inRing;
bool ringEndDelay;
bool hasLeftInflx = false, hasRightInflx = false;
bool ringInterval;
bool inCrossRoad;

int16_t barrierType;
int32_t barrierDistance;
bool aroundBarrier;

static bool IsRing(void);
static bool IsRingEnd(void);
static int16_t WhichCurve(void);
static bool IsCrossRoad(void);
static int16_t WhichBarrier(void);

static inline float Abs(float);
static inline float Min(float, float);
static inline float Max(float, float);
static inline bool OpstSign(int16_t, int16_t);
static inline bool InRange(int16_t value, int16_t lbound, int16_t hbound);

static int16_t black_pt_row;
static int16_t last_not_found_border_row;

bool OutOfRoadJudge() 
{
    int16_t cnt = 0;
    int16_t line = 0;
    for(int16_t row = 0; row < 10; ++row) 
    {
        for(int16_t col = 0; col < IMG_COL ; ++col) 
        {
            if(IsBlack(row, col)) 
                {
                ++cnt;
                }
        }
        if(cnt >= 200) 
        {
            ++line;
            if(line >= 4) 
            {
                return true;
            }
        }
        cnt = 0;
    }
    return false;
}

int16_t GetRoadType()
{
    if(ringEndDelay) {
        if(ringDistance < 10000) {
            return RingEnd;
        } else {
            ringDistance = 0;
            ringEndDelay = false;
            ringInterval = true;
        }
    } else if(ringInterval) {
        if(ringDistance > 20000) {
            ringDistance = 0;
            ringInterval = false;
        }
    } else if(inRing) {
        if(ringDistance > 250000) {
            ringDistance = 0;
            inRing = false;
        } else if(ringDistance > 15000 && IsRingEnd()) {
            ringDistance = 0;
            ringEndDelay = true;
            inRing = false;
//            BUZZLE_ON;
            return RingEnd;
        } else if(ringDistance < 5000) {
            return Ring;
        }
    } else if(inCrossRoad) {
        if(crossRoadDistance > 80000) {
            crossRoadDistance = 0;
            inCrossRoad = false;
        }
    } else if(aroundBarrier) {
        if(barrierDistance > 5000) {
            barrierDistance = 0;
            aroundBarrier = false;
        } else {
            return barrierType;
        }
    }
    
//    int16_t curve = WhichCurve();
    
    return /*curve != Unknown ? curve
        : */!inRing && !ringEndDelay && !ringInterval && !inCrossRoad && IsRing() ? Ring
        : !inRing && !ringEndDelay && IsCrossRoad() ? CrossRoad
        : !inRing && !ringEndDelay && !inCrossRoad ? WhichBarrier()
        : Unknown;
}

bool IsRing() 
{
//    int16_t row,temp;
//	


//    int16_t blackBlockRowsCnt = 0;
//    int16_t col;
//    int16_t whiteCol;
//    int16_t width;
//    for (int16_t row = IMG_ROW - 1; row >= 30; --row)
//    {
//        if(IsWhite(row, IMG_COL / 2))//滤掉中线白行
//        {
//            continue;
//        }
//        for(col = IMG_COL / 2 - 1; IsBlack(row, col) && col >= 0; --col) { }//滤掉该行中左边黑点
//        if(!InRange(col, 0, IMG_COL / 2 - 5))//黑跳白点是否在0到112-5范围内
//        {
//            continue;
//        }
//        for(whiteCol = col; IsWhite(row, whiteCol) && whiteCol >= 0; --whiteCol) { }//滤掉黑跳白点后的白点
//        width = col - whiteCol;//求出赛道宽度
//        for (col = IMG_COL / 2 + 1; IsBlack(row, col) && col < IMG_COL; ++col) { }//滤掉该行中右边黑点
//        if(InRange(col, IMG_COL / 2 + 4, IMG_COL))//黑跳白点是否在112+4到225范围内
//        {
//            for(whiteCol = col; IsWhite(row, whiteCol) && whiteCol < IMG_COL; ++whiteCol) { }//滤掉白点
//            if(Abs((whiteCol - col) - width) < 50) //求出左右赛道宽度差
//                {
//                    ++blackBlockRowsCnt;
//                }
//        }
//    }
//    
//    if(blackBlockRowsCnt > 2) 
//        {
//            for(row=20;row<IMG_ROW;row++)
//	{
//		if(
//			(resultSet.rightBorder[row-1] - resultSet.leftBorder[row-1] >220) && (IsWhite(row-1,IMG_COL/2))
//			&&(resultSet.rightBorder[row-2] - resultSet.leftBorder[row-2] >220) && (IsWhite(row-1,IMG_COL/2))
//			)
//			{
//				temp=row;
//				break;
//			}
//	}
//    
//	if(temp!=0)
//    {
//	for(row=temp;row<IMG_ROW;row++)
//	{
//		if(IsBlack(row, IMG_COL / 2))  //滤掉中间黑块
//		{
//			continue;
//		}
//		if(
//			(resultSet.rightBorder[row+1] - resultSet.leftBorder[row+1] > 190) && (IsWhite(row+1,IMG_COL/2))
//		  &&(resultSet.rightBorder[row+2] - resultSet.leftBorder[row+2] > 190) && (IsWhite(row+2,IMG_COL/2))
//		  )
//		return inRing = true;
//	}
//    }
//        }
//    return false;
int16_t cursor = 0;
    for(int16_t i = 0; i < 5; ++i) {
        cursor += resultSet.middleLine[i];
    }
    cursor /= 5;
    int16_t row;
    int16_t _cnt = 0;
    for(row = 5; row < 40; ++row) {
        if(IsBlack(row, cursor)) {
            break;
        }
        if(resultSet.rightBorder[row] - resultSet.leftBorder[row] > 200) {
            ++_cnt;
        }
    }
    if(_cnt < 10 || row == 40) {
        return false;
    }
    bool hasRightOffset = false;
    for(int16_t col = cursor; col < Min(cursor + 15, IMG_COL); ++col) {
        if(IsBlack(row - 1, col)) {
            hasRightOffset = true;
            cursor = col;
            --row;
            if(row == 0) {
                return false;
            }
        }
    }
    if(!hasRightOffset) {
        for(int16_t col = cursor; col > Max(cursor - 15, -1); --col) {
            if(IsBlack(row - 1, col)) {
                cursor = col;
                --row;
                if(row == 0) {
                    return false;
                }
            }
        }
    }
    int16_t left, right;
    int16_t cnt[4] = { 0 };
    int16_t minWidth, maxWidth;
    int16_t leftMost, rightMost;
    
    left = right = cursor;
    for(; IsBlack(row, left) && left > 0; --left) { }
    for(; IsBlack(row, right) && right < IMG_COL - 1; ++right) { }
    if(IsBlack(row, left) && IsBlack(row, right)) {
        return false;
    }
    leftMost = left;
    rightMost = right;
    minWidth = maxWidth = right - left;
    ++row;
    
    for(; row < IMG_ROW; ++row) {
        left = right = cursor;
        if(IsWhite(row, cursor)) {
            break;
        }
        for(; IsBlack(row, left) && left > 0; --left) { }
        for(; IsBlack(row, right) && right < IMG_COL - 1; ++right) { }
        if(IsBlack(row, left) && IsBlack(row, right)) {
            return false;
        }
        if(leftMost > left) {
            leftMost = left;
            ++cnt[0];
        } else if(leftMost < left) {
            ++cnt[1];
        }
        if(rightMost < right) {
            rightMost = right;
            ++cnt[2];
        } else if(rightMost > right) {
            ++cnt[3];
        }
        maxWidth = Max(right - left, maxWidth);
        cursor = (left + right) / 2;
    }
    
    return inRing =
//        #ifdef NO1
            leftMost < 80 && cnt[0] > 3 && cnt[1] > 3 && cnt[0] < 7
//        #else
//            rightMost >= IMG_COL - 80 && cnt[2] > 3 && cnt[3] > 3 && cnt[2] < 7
//        #endif
        /*&& Abs(cnt[0] - cnt[1]) < 4 */&& maxWidth > 75 && maxWidth - minWidth > 50;
}

bool IsRingEnd() 
{
    int16_t i,j,k;
    bool WhiteFlag[IMG_ROW];
    for(i=30;i<IMG_ROW;i++)
    {
        for(j=IMG_COL/2,k=IMG_COL/2;j>40 && k<180;j--,k++)
        {
            if(!(IsWhite(i,j) && IsWhite(i,k)))
            {
                WhiteFlag[i] = false;
                break;
            }
        }
        if(j==40 && k==180)
        {
            WhiteFlag[i] = true;
        }
    }
    
    for(i=30;i<IMG_ROW;i++)
    {
        if(WhiteFlag[i] && !WhiteFlag[i+1])
           {
            if(resultSet.rightBorder[i] - resultSet.leftBorder[i] > 100)
                return true;
           }
    }
    return false;
    
//	int16_t i,j,temp;
//	for(i=30;i<50;i++)
//		{
//			if(
//				(imgBuf[i][IMG_COL/2]&imgBuf[i+1][IMG_COL/2]&imgBuf[i+2][IMG_COL/2] == 1)
//			&&(resultSet.rightBorder[i-1] - resultSet.leftBorder[i-1] >180) && (IsWhite(i-1,resultSet.leftBorder[i-1])) && (IsWhite(i-1,resultSet.rightBorder[i-1]))
////			&&(resultSet.rightBorder[i-2] - resultSet.leftBorder[i-2] >220) && (IsWhite(i-2,IMG_COL/2))
//			)
//		return true;
//		}
//		
//		return false;	
//    double a = 9.0 / (resultSet.rightBorder[47] - resultSet.rightBorder[38]);
//    double b = 9.0 * resultSet.rightBorder[38] / (resultSet.rightBorder[38] - resultSet.rightBorder[47]);
//    int16_t r = 0;
//    for(int16_t row = 38; row < 48; ++row) {
////        if(IsBlack(row, resultSet.middleLine[row])) {
////            return false;
////        }
//        r += Abs(resultSet.rightBorder[row] - (row - 38 - b) / a);
//    }
//    return r < 10;
}

int16_t WhichCurve() 
{
    int16_t row;
    bool leftCurve = false;
    bool rightCurve = false;
    int16_t cnt = 0;
    for (row = 5; row < IMG_ROW && IsWhite(row, resultSet.middleLine[row]); ++row) { }
    if(row < IMG_ROW && !InRange(resultSet.middleLine[row], IMG_COL / 2 - 46, IMG_COL / 2 + 46)) 
    {
        black_pt_row = row;
        for(; row >= 0; --row) 
        {
            if(!resultSet.foundLeftBorder[row]) 
            {
                leftCurve = true;
                break;
            } 
            else if(!resultSet.foundRightBorder[row]) 
            {
                rightCurve = true;
                break;
            }
        }
        if(leftCurve) 
        {
            for(; row >= 0; --row) 
            {
                if(!resultSet.foundLeftBorder[row] && resultSet.foundRightBorder[row]) 
                {
                    ++cnt;
                    if(cnt > 5) 
                    {
                        return LeftCurve;
                    }
                }
            }
        } 
        else if(rightCurve) 
        {
            for(; row >= 0; --row) 
            {
                if(!resultSet.foundRightBorder[row] && resultSet.foundLeftBorder[row]) 
                {
                    ++cnt;
                    if(cnt > 5) 
                    {
                        return RightCurve;
                    }
                }
            }
        }
    }
    return Unknown;
}

bool IsCrossRoad() 
{
//    return resultSet.leftBorderNotFoundCnt > 2 && resultSet.rightBorderNotFoundCnt > 2
//        && resultSet.leftBorderNotFoundCnt + resultSet.rightBorderNotFoundCnt > 4;
    int16_t cnt = 0;
    for(int16_t i = 5; i < 30; ++i) {
        if(!resultSet.foundLeftBorder[i] && !resultSet.foundRightBorder[i]) {
            ++cnt;
        }
    }
    for(int16_t i = 30; i < 45; ++i) {
        if(IsBlack(i, resultSet.middleLine[i])
            || resultSet.middleLine[i] > IMG_COL - 60
            || resultSet.middleLine[i] < 60) {
            return false;
        }
    }
    return inCrossRoad = cnt > 6;
}

int16_t WhichBarrier() 
{
    int16_t inRow;
    int16_t outRow;
    int16_t row;
    int16_t _barrierType;
    for(row = 10; row < 45 && Abs(resultSet.middleLine[row] - resultSet.middleLine[row - 2]) <= 16; ++row) { }
    if(!InRange(resultSet.middleLine[row - 2], IMG_COL / 2 - 20, IMG_COL / 2 + 20)) 
    {
        return Unknown;
    }
    
    _barrierType = resultSet.middleLine[row] - resultSet.middleLine[row - 2] > 0 ? LeftBarrier : RightBarrier;
    
    inRow = row;
    row += 2;
    for(; row < IMG_ROW && Abs(resultSet.middleLine[row] - resultSet.middleLine[row - 2]) <= 10; ++row) { }
    if((resultSet.middleLine[row] - resultSet.middleLine[row - 2] > 0 && _barrierType == LeftBarrier)
        || (resultSet.middleLine[row] - resultSet.middleLine[row - 2] < 0 && _barrierType == RightBarrier)) 
    {
        return Unknown;
    }
    outRow = row;
    if(outRow - inRow > 5 && row < IMG_ROW && resultSet.middleLine[row] > IMG_COL / 2 - 20
        && resultSet.middleLine[row] < IMG_COL / 2 + 20) 
    {
        aroundBarrier = true;
        return barrierType = _barrierType;
    } 
    else 
    {
        return Unknown;
    }
}

void RingCompensateGoLeft() 
{
//    int row;
//    for (row = IMG_ROW - 1;
//        row >= 30 && IsWhite(row, IMG_COL / 2); --row) { }
//    for (; row >= 10 && IsBlack(row, IMG_COL / 2); --row) { }
//    int col;
//    for (col = IMG_COL / 2; col >= 0 && IsBlack(row + 1, col); --col) { }
//    for(int i = row; i > 0; --i)
//    {
//        resultSet.rightBorder[i] = resultSet.leftBorder[i] ;
//        resultSet.middleLine[i] = (resultSet.leftBorder[i] + resultSet.rightBorder[i]) / 2;
//    }
//    int16_t borderSearchStart = col / 2;
//    for (int i = row; i < IMG_ROW; ++i)
//    {
//        LeftBorderSearchFrom(i, borderSearchStart);
//        RightBorderSearchFrom(i, borderSearchStart);
//        resultSet.middleLine[i] = (resultSet.leftBorder[i] + resultSet.rightBorder[i]) / 2;
//        borderSearchStart = resultSet.middleLine[i];
//    }
    for (int i=MODE.pre_sight-3; i < MODE.pre_sight+3; ++i) {
			resultSet.middleLine[i] = resultSet.leftBorder[i] + 60;// - ring_offset;
		}

}

void RingCompensateGoRight()
{
//    int row;
//    for (row = IMG_ROW - 1;row >= 30 && IsWhite(row, IMG_COL / 2); --row) { }
//    for (; row >= 10 && IsBlack(row, IMG_COL / 2); --row) { }
//    int col;
//    for (col = IMG_COL / 2; col < IMG_COL && IsBlack(row + 1, col); ++col) { }
//    for (int i = row; i > 0; --i)
//    {
//        resultSet.leftBorder[i] = col - (row + 1 - i) * (col - resultSet.leftBorder[0]) / (1*row);
//        resultSet.middleLine[i] = (resultSet.leftBorder[i] + resultSet.rightBorder[i]) / 2;
//    }
//    int16_t borderSearchStart = col + (IMG_COL - col) / 2;
//    for (int i = row; i < IMG_ROW; ++i)
//    {
//        LeftBorderSearchFrom(i, borderSearchStart);
//        RightBorderSearchFrom(i, borderSearchStart);
//        resultSet.middleLine[i] = (resultSet.leftBorder[i] + resultSet.rightBorder[i]) / 2;
//        borderSearchStart = resultSet.middleLine[i];
//    }
//	
//	for(int i = MODE.pre_sight-3;i < MODE.pre_sight+3;++i) {
//		resultSet.middleLine[i] = resultSet.rightBorder[i] - 80;
//	}
    for (int i=MODE.pre_sight-3; i < MODE.pre_sight+3; ++i) {
			resultSet.middleLine[i] = resultSet.rightBorder[i] - 60;// + ring_offset;
		}
//     for(int i=0;i < IMG_ROW;i++)
//     {
//         resultSet.middleLine[i] = resultSet.rightBorder[i] - 40;
//     }
//    for(int i = MODE.pre_sight-3;i< MODE.pre_sight;i++)
//    {
//        resultSet.middleLine[i] = resultSet.middleLine[MODE.pre_sight-3] + i;
//    }

}

void RingEndCompensateFromLeft() 
{
//    int16_t row;
//    int16_t col;
//    for(row = last_not_found_border_row; row < IMG_ROW; ++row) 
//    {
//        if(resultSet.rightBorder[row] < IMG_COL - 40) 
//        {
//            col = resultSet.rightBorder[row];
//            break;
//        }
//    }
//    if(row < IMG_ROW) 
//    {
//        --row;
//        for(; row >= 0; --row) 
//        {
//            resultSet.rightBorder[row] = col;
//        }
//        MiddleLineUpdateAll();
//    }
    for (int i=MODE.pre_sight-3; i < MODE.pre_sight+3; ++i) {
			resultSet.middleLine[i] = resultSet.leftBorder[i] + 50;// - ring_offset;
		}

}

void RingEndCompensateFromRight() 
{
//    int16_t row;
//    int16_t col;
//    for(row = last_not_found_border_row; row < IMG_ROW; ++row) 
//    {
//        if(resultSet.leftBorder[row] > 40) 
//        {
//            col = resultSet.leftBorder[row];
//            break;
//        }
//    }
//    if(row < IMG_ROW) 
//    {
//        --row;
//        for(; row >= 0; --row) 
//        {
//            resultSet.leftBorder[row] = col;
//        }
//        MiddleLineUpdateAll();
//    }
//    BUZZLE_ON;
    for (int i=MODE.pre_sight-3; i < MODE.pre_sight+3; ++i) {
			resultSet.middleLine[i] = resultSet.rightBorder[i] - 50;// + ring_offset;
		}
//	for(int i = MODE.pre_sight-3;i < MODE.pre_sight+3;++i) {
//		resultSet.middleLine[i] = resultSet.rightBorder[i] - 50;
//	}

}

void LeftCurveCompensate() 
{
    for (int row_ = IMG_ROW - 1; row_ > black_pt_row; --row_)
    {
        resultSet.middleLine[row_] = 0;
//			printf("LeftCurveCompensate111111\n");
    }
    for (int i=MODE.pre_sight; i < MODE.pre_sight+3; ++i)
    {
        resultSet.middleLine[i] =resultSet.middleLine[MODE.pre_sight]-resultSet.middleLine[MODE.pre_sight]*(i-MODE.pre_sight)/8;
//			printf("LeftCurveCompensate222222\n");
    }
//	for(int row=0;row<IMG_ROW;row++) 
//  {
//		resultSet.middleLine[row] = 20;
//	}
}

void RightCurveCompensate() 
{
    for (int row_ = IMG_ROW - 1; row_ > black_pt_row; --row_)
    {
        resultSet.middleLine[row_] = IMG_COL - 1;
//			printf("RightCurveCompensate111111\n");
    }
    for (int i=MODE.pre_sight; i < MODE.pre_sight+3; ++i)
    {
        resultSet.middleLine[i] = resultSet.middleLine[MODE.pre_sight]+(IMG_COL-1-resultSet.middleLine[MODE.pre_sight])*(i-MODE.pre_sight)/8;
//			printf("RightCurveCompensate222222\n");
    }
}

void CrossRoadCompensate() 
{
    int16_t leftCompensateStart;
    int16_t rightCompensateStart;
    int16_t leftCompensateEnd = IMG_ROW;
    int16_t rightCompensateEnd = IMG_ROW;
    {
        int row = 6;
        while (row < IMG_ROW && resultSet.leftBorder[row] != 0
            && Abs(resultSet.leftSlope[row] - resultSet.leftSlope[row - 1]) < 3
            && !OpstSign(resultSet.leftSlope[row], resultSet.leftSlope[row - 1])) { ++row; }
        leftCompensateStart = Min(row, IMG_ROW - 1);
//        row += 5;
//        while (row < IMG_ROW
//            && (resultSet.leftBorder[row] == 0 || Abs(resultSet.leftSlope[row] - resultSet.leftSlope[row - 1]) >= 3)) { ++row; }
//        row += 4;
//        leftCompensateEnd = Min(row, IMG_ROW - 1);
    }

    {
        int row = 6;
        while (row < IMG_ROW && resultSet.rightBorder[row] != IMG_COL - 1
            && Abs(resultSet.rightSlope[row] - resultSet.rightSlope[row - 1]) < 3
            && !OpstSign(resultSet.rightSlope[row], resultSet.rightSlope[row - 1])) { ++row; }
        rightCompensateStart = Min(row, IMG_ROW - 1);
//        row += 5;
//        while (row < IMG_ROW
//            && (resultSet.rightBorder[row] == IMG_COL - 1 || Abs(resultSet.rightSlope[row] - resultSet.rightSlope[row - 1]) >= 3)) { ++row; }
//        row += 4;
//        rightCompensateEnd = Min(row, IMG_ROW - 1);
    }

    for (int row = leftCompensateStart; row < leftCompensateEnd; ++row)
    {
        resultSet.leftBorder[row] = row * resultSet.leftSlope[leftCompensateStart - 5] + resultSet.leftZero[leftCompensateStart - 5];
        if(IsBlack(row, resultSet.leftBorder[row])) 
        {
            leftCompensateEnd = row;
            break;
        }
    }

    for (int row = rightCompensateStart; row < rightCompensateEnd; ++row)
    {
        resultSet.rightBorder[row] = row * resultSet.rightSlope[rightCompensateStart - 5] + resultSet.rightZero[rightCompensateStart - 5];
        if(IsBlack(row, resultSet.rightBorder[row])) 
        {
            rightCompensateEnd = row;
            break;
        }
    }
    
    int16_t borderSearchStart;
    
    if (leftCompensateEnd < rightCompensateEnd)
    {
        borderSearchStart = (resultSet.leftBorder[leftCompensateEnd - 1] + resultSet.rightBorder[leftCompensateEnd - 1]) / 2;
        for (int16_t row = leftCompensateEnd; row < rightCompensateEnd; ++row)
        {
            LeftBorderSearchFrom(row, borderSearchStart);
            borderSearchStart = (resultSet.rightBorder[row] + resultSet.leftBorder[row]) / 2;
        }
    }
    else if (leftCompensateEnd > rightCompensateEnd)
    {
        borderSearchStart = (resultSet.leftBorder[rightCompensateEnd - 1] + resultSet.rightBorder[rightCompensateEnd - 1]) / 2;
        for (int16_t row = rightCompensateEnd; row < leftCompensateEnd; ++row)
        {
            RightBorderSearchFrom(row, borderSearchStart);
            borderSearchStart = (resultSet.rightBorder[row] + resultSet.leftBorder[row]) / 2;
        }
    }

    int16_t compensateEnd = Max(leftCompensateEnd, rightCompensateEnd);

    borderSearchStart = (resultSet.leftBorder[compensateEnd - 1] + resultSet.rightBorder[compensateEnd - 1]) / 2;
    for (int row = compensateEnd; row < IMG_ROW; ++row)
    {
        LeftBorderSearchFrom(row, borderSearchStart);
        RightBorderSearchFrom(row, borderSearchStart);
        if (Abs((resultSet.rightBorder[row] + resultSet.leftBorder[row]) - (resultSet.rightBorder[row - 1] + resultSet.leftBorder[row - 1])) < 10)
        {
            borderSearchStart = (resultSet.rightBorder[row] + resultSet.leftBorder[row]) / 2;
        }
    }
    
    MiddleLineUpdateAll();
}

bool StartLineJudge(int16_t row) 
{
    int16_t toggleCnt = 0;
    int16_t patternRowCnt = 0;
    for(int16_t i = row; i >= row - 6; --i) 
    {
        for(int16_t j = IMG_COL / 2; j >= 0; --j) 
        {
            if(TstImgBufAsBitMap(i, j) != TstImgBufAsBitMap(i, j+1)) 
            {
                if(toggleCnt > 14) 
                {
                    toggleCnt = 0;
                    ++patternRowCnt;
                    if(patternRowCnt > 4) 
                    {
                        return true;
                    } 
                    else 
                    {
                        break;
                    }
                } 
                else 
                {
                    ++toggleCnt;
                }
            }
        }
    }
    return false;
}

bool StraightLineJudge(void) 
{
    int16_t middleAreaCnt = 0;
    for(int16_t i = 0; i < IMG_ROW; ++i) 
    {
        if(InRange(resultSet.middleLine[i], IMG_COL / 2 - 10, IMG_COL / 2 + 10)) 
        {
            middleAreaCnt++;
        }
    }
    return middleAreaCnt > 20;
}

inline float Abs(float input) 
{
    return input >= 0 ? input : -input;
}

inline float Min(float a, float b) 
{
    return a > b ? b : a;
}

inline float Max(float a, float b) 
{
    return a > b ? a : b;
}

inline bool OpstSign(int16_t a, int16_t b) 
{
    return (a & 0x8000) ^ (b & 0x8000);
}

inline bool InRange(int16_t value, int16_t lbound, int16_t hbound) 
{
    return value > lbound && value < hbound;
}
