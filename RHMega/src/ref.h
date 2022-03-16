#ifndef RHM_REF_H
#define RHM_REF_H

extern "C" float ref_signal[];

extern "C" float lerp(float a, float b, float alpha);

extern "C" float unlerp(float low, float high, float val);

extern "C" float remap(float low_old, float high_old, float val, float low, float high);

extern "C" void init_ref();

extern "C" float ref(unsigned long time_ms);

#endif