#include "renderer/vertexBuffer.hpp"
#include "glHelper.hpp"

#include <GL/glew.h>
#include <vector>
#include <algorithm>

using namespace photon::renderer;

struct vertexDataSpec
{
   GLint size;
   GLenum type;
   GLboolean normalised;
   size_t byteSize;
};

template<typename T> vertexDataSpec typeToSpec();
template<> vertexDataSpec typeToSpec<GLfloat>(){return {1,GL_FLOAT,GL_FALSE, sizeof(GLfloat)};}
template<> vertexDataSpec typeToSpec<GLint  >(){return {1,GL_INT  ,GL_FALSE, sizeof(GLint)};}

template<typename... T>
vertexBuffer<T...>::vertexBuffer()
{
   // create buffers
   GLint bufers[2];
   GLC(glGenBuffers(2,bufers));
   vbId = bufers[0];
   ibId = bufers[1];

   // setup vertex buffer based on templet
   {
      std::vector<vertexDataSpec> vdc = {typeToSpec<T>()...};
      GLC(glBindBuffer(GL_ARRAY_BUFFER,vbId));

      GLint stride = 0;
      for(auto& i : vdc) stride += i.size;
      vertexSize = stride;
      int byteOfset = 0;
      for(int i = 0; i < vdc.size();i++)
      {
         GLC(glEnableVertexAttribArray(i));
         GLC(glVertexAttribPointer(
            i,
            vdc[i].size,
            vdc[i].type,
            vdc[i].normalised,
            vertexSize,
            byteOfset
         ));
         byteOfset += vdc[i].size * vdc[i].byteSize; //temporary replace with proper value
      }
   }
}

template<typename... T>
void vertexBuffer<T...>::loadData(const vertexBufferData& toLoad)
{
   GLC(glBufferData(GL_ARRAY_BUFFER, toLoad.vertexCount * vertexSize,           vbId, GL_DYNAMIC_DRAW));
   GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, toLoad.indexCount  * sizeof(unsigned int), ibId, GL_DYNAMIC_DRAW));
}

template<typename... T>
void vertexBuffer<T...>::bind()
{
   GLC(glBindBuffer(GL_ARRAY_BUFFER,vbId));
   GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbId));
}

template<typename... T>
void vertexBuffer<T...>::unbind()
{
   GLC(glBindBuffer(GL_ARRAY_BUFFER,0));
   GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}
