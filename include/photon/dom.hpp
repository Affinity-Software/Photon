#pragma once
#include <vector>
#include <string>

//TODO proper dom interface
namespace photon{
   namespace dom{
      typedef unsigned int id;
      enum callbackTypes{};
      struct event{};
      struct callback
      {
         callbackTypes type;
         void (*Callback)(event);
      };


      struct node
      {
         id parent;
         id tag;
         std::vector<id> children;
         std::vector<callback> callbacs;
      };
   }

   class _dom
   {
   private:
      struct impl;
      impl* pimpl;
   public:
      _dom();
      ~_dom();
      void insertNode(const dom::node& toinsert);
      void insertNode(const _dom& toinsert);
      void deleteNode(const dom::id& id);
      std::vector<dom::id> getNodesByTag(const std::string& tag);//TODO implament this
   };
}