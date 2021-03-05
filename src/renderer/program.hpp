#pragma once

#include "GL/glew.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

namespace photon::renderer{
class program
{
private:
   GLint id;
   std::vector<GLuint> shaderIds;
   std::unordered_map<std::string,GLuint> uniformNames;
   void addShader(const std::vector<std::string>& source,GLint type);
public:
   program(const std::string& sources);
   ~program();
   void bind() const;
   void unbind() const;
   void setUniform(std::string name ,glm::fvec1 value);
   void setUniform(std::string name ,glm::fvec2 value);
   void setUniform(std::string name ,glm::fvec3 value);
   void setUniform(std::string name ,glm::fvec4 value);
   void setUniform(std::string name ,glm::ivec1 value);
   void setUniform(std::string name ,glm::ivec2 value);
   void setUniform(std::string name ,glm::ivec3 value);
   void setUniform(std::string name ,glm::ivec4 value);
   void setUniform(std::string name ,glm::mat2 value);
   void setUniform(std::string name ,glm::mat3 value);
   void setUniform(std::string name ,glm::mat4 value);
   void setUniform(std::string name ,glm::mat2x3 value);
   void setUniform(std::string name ,glm::mat3x2 value);
   void setUniform(std::string name ,glm::mat2x4 value);
   void setUniform(std::string name ,glm::mat4x2 value);
   void setUniform(std::string name ,glm::mat3x4 value);
   void setUniform(std::string name ,glm::mat4x3 value);
   void setUniform(std::string name , std::vector<glm::fvec1>);
   void setUniform(std::string name , std::vector<glm::fvec2>);
   void setUniform(std::string name , std::vector<glm::fvec3>);
   void setUniform(std::string name , std::vector<glm::fvec4>);
   void setUniform(std::string name , std::vector<glm::ivec1>);
   void setUniform(std::string name , std::vector<glm::ivec2>);
   void setUniform(std::string name , std::vector<glm::ivec3>);
   void setUniform(std::string name , std::vector<glm::ivec4>);
};
}