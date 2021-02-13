#include "glHelper.hpp"
#include "GL/glew.h"

#include <iostream>
#include <cassert>

void checkGlError()
{
    GLenum err;
    while ( err = glGetError() )
    {
        if(err != GL_NO_ERROR)
        {
            std::cout << gluErrorString(err) << std::endl;
            assert(true);
        }
    }
}