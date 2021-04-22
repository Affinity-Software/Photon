#pragma once
#include <GL/glew.h>
#include <string>

void checkGlError(const std::string& file, const int path);

#ifdef DEBUG
    #define GLC(x) x;checkGlError(__FILE__,__LINE__);
#else
    #define GLC(x) x;
#endif