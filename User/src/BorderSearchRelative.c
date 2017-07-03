#include "BorderSearchRelative.h"
#include "PatternMatch.h"
#include "ImgProc.h"
#include "gpio.h"

bool LeftBorderSearchFrom(int16_t row, int16_t startIndex) 
{
    for(int16_t j = startIndex; j >= 0; --j) 
        {
            if(IsBlack(row, j)) 
                {
                    resultSet.leftBorder[row] = j;
                    return true;
                }
        }
    resultSet.leftBorder[row] = 0;
    ++resultSet.leftBorderNotFoundCnt;
//		if(resultSet.imgProcFlag == CROSS_ROAD)
//			CrossRoadCompensate();
    return false;
}

bool RightBorderSearchFrom(int16_t row, int16_t startIndex) 
{
    for(int16_t j = startIndex; j < IMG_COL; ++j) 
        {
            if(IsBlack(row, j)) 
                {
                    resultSet.rightBorder[row] = j;
                    return true;
                }
        }
//         else if(resultSet.imgProcFlag == CROSS_ROAD)
//                CrossRoadCompensate();
    resultSet.rightBorder[row] = IMG_COL - 1;
    ++resultSet.rightBorderNotFoundCnt;
//		if(resultSet.imgProcFlag == CROSS_ROAD)
//			CrossRoadCompensate();
    return false;
}

void MiddleLineUpdate(int16_t row)
{
//	if(resultSet.imgProcFlag == CIRCLE)
//                RingCompensateGoRight();
//	resultSet.middleLine[row] = resultSet.rightBorder[row] - 65;
//    else if(resultSet.imgProcFlag == RINGEND)
//                RingEndCompensateFromRight();
//              else 
                resultSet.middleLine[row] = (resultSet.leftBorder[row] + resultSet.rightBorder[row]) / 2;
}

void MiddleLineUpdateAll() 
{
    for(int row = 0; row < IMG_ROW; ++row) 
        {
            resultSet.middleLine[row] = (resultSet.leftBorder[row] + resultSet.rightBorder[row]) / 2;
        }
}
