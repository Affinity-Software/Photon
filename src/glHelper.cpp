#include "glHelper.hpp"

#include <iostream>
#include <cassert>

static std::string glErorTostring(GLenum err)
{
    if(err == GL_INVALID_ENUM)
        return "GL_INVALID_ENUM";
    else if(err == GL_INVALID_VALUE)
        return "GL_INVALID_VALUE";
    else if(err == GL_INVALID_OPERATION)
        return "GL_INVALID_OPERATION";
    else if(err == GL_STACK_OVERFLOW)
        return "GL_STACK_OVERFLOW";
    else if(err == GL_STACK_UNDERFLOW)
        return "GL_STACK_UNDERFLOW";
    else if(err == GL_OUT_OF_MEMORY)
        return "GL_OUT_OF_MEMORY";
    else if(err == GL_INVALID_FRAMEBUFFER_OPERATION)
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    else if(err == GL_CONTEXT_LOST)
        return "GL_CONTEXT_LOST";
    else if(err == GL_TABLE_TOO_LARGE)
        return "GL_TABLE_TOO_LARGE1";
    else
        return "unkown error";
}

void checkGlError(const std::string& file, const int line)
{
    GLenum err;
    while ( (err = glGetError()) != GL_NO_ERROR )
    {
        if(err != GL_NO_ERROR)
        {
            std::cout << "[ERROR] [opengl] in file " << file<< " at line " << line << " with code "<< err << " " <<  glErorTostring(err) << std::endl;
            assert(true);
        }
    }
}