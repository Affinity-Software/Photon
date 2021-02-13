#pragma once

#include "GL/glew.h"

#include <string>
#include <vector>
#include <string>

namespace photon::renderer{
class program
{
private:
   GLint id;
   std::vector<GLint> shaderIds;
   void addShader(const std::vector<std::string>& source,GLenum type);
public:
   program(const std::vector<std::string>& sources);
   ~program();
};
}