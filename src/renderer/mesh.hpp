#pragma once
#include "glHelper.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

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
      bool strideCheck(size_t size);
   };
   

struct vertexDataSpec
{
   GLint size;
   GLenum type;
   GLboolean normalised;
   size_t typeSize;
   size_t arraySize;
};

template<typename T> vertexDataSpec typeToSpec();

//code from stack overflow https://stackoverflow.com/a/16337657/13248064

template<typename>
struct is_std_array : std::false_type {};

template<typename T, size_t A>
struct is_std_array<std::array<T,A>> : std::true_type {};

template<typename T> vertexDataSpec arrayTypeToSpec()
{
   if constexpr(is_std_array<T>::value)
   {
      auto temp = typeToSpec<std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>>();
      temp.arraySize = std::tuple_size<T>::value;
      return temp;
   }
   else
   {
      return typeToSpec<T>();
   }
}

template<typename... T>
mesh<T...>::mesh()
{
   // create buffers
   GLuint bufers[2];
   GLC(glGenBuffers(2,bufers));
   vbId = bufers[0];
   ibId = bufers[1];

   // setup vertex buffer based on templet
   const std::vector<vertexDataSpec> vdc = {arrayTypeToSpec<T>()...};
   GLC(glBindBuffer(GL_ARRAY_BUFFER,vbId));

   GLint stride = 0;
   for(auto& i : vdc) stride += i.size * i.typeSize * i.arraySize;
   vertexSize = stride;
   GLintptr byteOfset = 0;
   size_t index = 0;
   for(size_t i = 0; i < vdc.size();i++)
   {
      for(size_t j = 0; j < vdc[i].arraySize; j++)
      {
         GLC(glEnableVertexAttribArray(index));
         GLC(glVertexAttribPointer(
            index,
            vdc[i].size,
            vdc[i].type,
            vdc[i].normalised,
            vertexSize,
            (void*)byteOfset
         ));
         index++;
         byteOfset += vdc[i].size * vdc[i].typeSize; 
      }
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

template<typename... T>
bool mesh<T...>::strideCheck(size_t size)
{
   return vertexSize == size;
}

}