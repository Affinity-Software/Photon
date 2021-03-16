#pragma once
#include "glHelper.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace photon::renderer
{
   struct vertexBufferData
   {
      void* vertexData;
      size_t vertexCount;
      void* indexData;
      size_t indexCount;
   };

   //this templet should be replaced to be based on a struct 
   template<typename... T>
   class mesh
   {
   private:
      GLuint vbId;
      size_t vbSize;
      GLuint ibId;
      size_t ibSize;
      size_t vertexSize; // in bytes
   public:
      mesh();
      ~mesh();
      template<typename VT>
      void loadData(const std::vector<VT>& vertexData, const std::vector<unsigned int>& indexData);
      void bind() const;
      void unbind() const;
      int getIndexCount() const;
   };
   

struct vertexDataSpec
{
   GLint size;
   GLenum type;
   GLboolean normalised;
   size_t byteSize;
};

template<typename T> vertexDataSpec typeToSpec();

template<typename... T>
mesh<T...>::mesh()
{
   // create buffers
   GLuint bufers[2];
   GLC(glGenBuffers(2,bufers));
   vbId = bufers[0];
   ibId = bufers[1];

   // setup vertex buffer based on templet
   const std::vector<vertexDataSpec> vdc = {typeToSpec<T>()...};
   GLC(glBindBuffer(GL_ARRAY_BUFFER,vbId));

   GLint stride = 0;
   for(auto& i : vdc) stride += i.size * i.byteSize;
   vertexSize = stride;
   GLintptr byteOfset = 0;
   for(size_t i = 0; i < vdc.size();i++)
   {
      GLC(glEnableVertexAttribArray(i));
      GLC(glVertexAttribPointer(
         i,
         vdc[i].size,
         vdc[i].type,
         vdc[i].normalised,
         vertexSize,
         (void*)byteOfset
      ));
      byteOfset += vdc[i].size * vdc[i].byteSize; 
   }
}

template<typename... T>
mesh<T...>::~mesh() 
{
   GLC(glDeleteBuffers(1,&vbId));
   GLC(glDeleteBuffers(1,&ibId));
}

template<typename... T>
template<typename VT>
void mesh<T...>::loadData(const std::vector<VT>& vertexData, const std::vector<unsigned int>& indexData)
{	
   vbSize = vertexData.size();
   GLC(glBufferData(
      GL_ARRAY_BUFFER,
      vbSize * vertexSize,
      vertexData.data(),
      GL_DYNAMIC_DRAW
    ));
   ibSize = indexData.size();
   GLC(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      ibSize * sizeof(unsigned int),
      indexData.data(),
      GL_DYNAMIC_DRAW
    ));
}

template<typename... T>
void mesh<T...>::bind() const
{
   GLC(glBindBuffer(GL_ARRAY_BUFFER,vbId));
   GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId));
}

template<typename... T>
void mesh<T...>::unbind() const
{
   GLC(glBindBuffer(GL_ARRAY_BUFFER,0));
   GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}

template<typename... T>
int mesh<T...>::getIndexCount() const
{
   return ibSize;
}

}