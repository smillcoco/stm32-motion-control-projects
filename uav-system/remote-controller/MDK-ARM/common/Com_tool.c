#include "Com_tool.h"


int16_t Com_limit(int16_t value, int16_t min, int16_t max)
{
    int16_t result = value;
    if(value < min)
    {
        result = min;
    }
    else if(value > max)
    {
        result = max;
    }
    return result;

}
