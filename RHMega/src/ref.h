#ifndef RHM_REF_H
#define RHM_REF_H

#define REF_LENTGH_MS (10000.0)

extern "C" float ref_signal[] = {
    0,
    1000,
    0
};

extern "C" void init_ref();

extern "C" float ref(float time_ms);

#endif