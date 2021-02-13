#pragma once

#ifdef DEBUG
    #define GLC(x) x;checkGlError();
#else
    #define GLC(x) x;
#endif

void checkGlError();