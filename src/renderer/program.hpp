#pragma once

#include "GL/glew.h"

#include <string>
#include <vector>

namespace photon::renderer{
class program
{
private:
   GLint id;
   std::vector<GLint> shaderIds;
   void addShader(const std::vector<std::string>& source,GLint type);
public:
   program(const std::string& sources);
   ~program();
   void bind() const;
   void unbind() const;
};
}