#pragma once
#include <GL/glew.h>
#include <string>

#ifdef DEBUG
    #define GLC(x) x;checkGlError(__FILE__,__LINE__);
#else
    #define GLC(x) x;
#endif

void checkGlError(const std::string& file, const int path);