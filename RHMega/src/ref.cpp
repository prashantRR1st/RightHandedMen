#include <Arduino.h>
#include "ref.h"

#define REF_LENTGH_MS (1430.0)

extern "C" float ref_signal[] = {
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    101,
    106,
    111,
    123,
    149,
    159,
    186,
    104,
    56,
    163,
    91,
    265,
    439,
    494,
    618,
    843,
    930,
    1347,
    1612,
    1874,
    2453,
    2633,
    3508,
    4086,
    4369,
    4905,
    2456,
    4849,
    7243,
    7846,
    8118,
    8791,
    9226,
    9578,
    9828,
    9830,
    9792,
    9346,
    9085,
    8465,
    8103,
    6834,
    5904,
    4086,
    3468,
    2850,
    2491,
    1547,
    1198,
    848,
    417,
    313,
    224,
    95,
    100
};

unsigned long size;

extern "C" float lerp(float a, float b, float alpha)
{
    return (1.0 - alpha) * a + alpha * b;
}

extern "C" float unlerp(float low, float high, float val)
{
    return (val - low) / (high - low);
}

extern "C" float remap(float low_old, float high_old, float val, float low, float high)
{
    return lerp(low, high, unlerp(low_old, high_old, val));
}

extern "C" void init_ref()
{
    size = sizeof(ref_signal) / sizeof(ref_signal[0]);
}

extern "C" float ref(unsigned long time_ms)
{
    float timeFactor = ((size - 1) * (float)time_ms) / REF_LENTGH_MS;
    unsigned long idx = (unsigned long)floor(timeFactor);
    float alpha = timeFactor - idx;
    if (idx >= size - 1)
    {
        return ref_signal[size - 1];
    }
    return lerp(ref_signal[idx], ref_signal[idx + 1], alpha);
}