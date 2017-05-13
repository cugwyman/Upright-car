#ifndef __BORDER_SEARCH_RELATIVE
#define __BORDER_SEARCH_RELATIVE

#include "root.h"

bool LeftBorderSearchFrom(int16_t row, int16_t startIndex);
bool RightBorderSearchFrom(int16_t row, int16_t startIndex);
void MiddleLineUpdate(int16_t row);
void MiddleLineUpdateAll(void);

#endif
