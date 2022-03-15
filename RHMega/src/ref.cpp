#include <Arduino.h>
#include "ref.h"

int size;

float lerp(float a, float b, float alpha){
    return (1.0-alpha) * a + alpha * b;
}

extern "C" void init_ref(){
    size = sizeof(ref_signal)/sizeof(ref_signal[0]);
}

extern "C" float ref(float time_ms){
    float timeFactor = ((size-1) * time_ms) / REF_LENTGH_MS;
    int idx = (int) floor(timeFactor);
    float alpha = timeFactor - idx;
    if(idx >= size - 1){
        return ref_signal[size-1];
    }
    return lerp(ref_signal[idx], ref_signal[idx+1], alpha);
}