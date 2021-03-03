#include "renderer/mesh.hpp"

template<> photon::renderer::vertexDataSpec photon::renderer::typeToSpec<GLfloat  >(){return {1,GL_FLOAT,GL_FALSE, sizeof(GLfloat)};}
template<> photon::renderer::vertexDataSpec photon::renderer::typeToSpec<GLint    >(){return {1,GL_INT  ,GL_FALSE, sizeof(GLint)  };}
template<> photon::renderer::vertexDataSpec photon::renderer::typeToSpec<glm::vec2>(){return {2,GL_FLOAT,GL_FALSE, sizeof(GLfloat)};}
template<> photon::renderer::vertexDataSpec photon::renderer::typeToSpec<glm::vec4>(){return {4,GL_FLOAT,GL_FALSE, sizeof(GLfloat)};}