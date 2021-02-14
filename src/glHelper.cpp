#include "glHelper.hpp"
#include <GL/glew.h>

#include <iostream>
#include <cassert>

void checkGlError()
{
    GLenum err;
    while ( (err = glGetError()) == GL_NO_ERROR )
    {
        if(err != GL_NO_ERROR)
        {
            std::cout << glewGetErrorString(err) << std::endl;
            assert(true);
        }
    }
}