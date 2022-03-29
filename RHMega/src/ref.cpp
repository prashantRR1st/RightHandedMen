#include <Arduino.h>
#include "ref.h"

#define REF_LENTGH_MS (2204.47)

extern "C" float ref_signal[] = {    
        103,
        102,
        102,
        101,
        100,
        100,
        100,
        99,
        102,
        104,
        106,
        111,
        114,
        116,
        122,
        124,
        131,
        134,
        147,
        152,
        128,
        155,
        181,
        181,
        170,
        163,
        136,
        134,
        136,
        144,
        203,
        230,
        330,
        353,
        360,
        363,
        235,
        309,
        382,
        420,
        491,
        541,
        600,
        353,
        686,
        1020,
        1158,
        1284,
        1352,
        1426,
        1696,
        1950,
        2112,
        2480,
        2887,
        1496,
        2799,
        4102,
        4706,
        5028,
        5678,
        6284,
        3192,
        5379,
        7566,
        8110,
        8378,
        8918,
        9534,
        11058,
        11610,
        11827,
        12421,
        12558,
        12667,
        12673,
        12653,
        12463,
        12258,
        11510,
        11247,
        10613,
        10230,
        9445,
        8660,
        8225,
        6882,
        6457,
        5555,
        5081,
        3773,
        3393,
        2108,
        1817,
        1245,
        979,
        390,
        273,
        191,
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