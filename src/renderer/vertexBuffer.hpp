#include <memory>

namespace photon::renderer
{
   struct vertexBufferData
   {
      std::shared_ptr<void> vertexData;
      size_t vertexCount;
      std::shared_ptr<unsigned int> indexData;
      size_t indexCount;
   };

   //this templet should be replaced to be based on a struct 
   template<typename... T>
   class vertexBuffer
   {
   private:
      int vbId;
      int ibId;
      const size_t vertexSize; // in bytes
   public:
      vertexBuffer();
      ~vertexBuffer();
      void loadData(const vertexBufferData& toLoad);
      void bind();
      void unbind();
   };
   
}