#pragma once
#include <vector>

namespace photonGui{
   class _dom
   {
   private:
      struct impl;
      impl* pimpl;
   public:
      _dom();
      ~_dom();
      void insertElement(const dom::element& toinsert);
      void insertElement(const _dom& toinsert);
      void deleteElement(const dom::id& id);
   };

   namespace dom{
      typedef u_int32_t id;
      enum callbackTypes{};
      struct event{};
      struct callback
      {
         callbackTypes type;
         void (*Callback)(event);
      };


      struct element
      {
         id parent;
         std::vector<id> children;
         std::vector<callback> callbacs;
      };
   }
}