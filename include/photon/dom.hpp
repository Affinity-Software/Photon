#pragma once
#include <vector>
#include <string>
#include <memory>

//TODO proper dom interface
namespace photon{
   class _dom;
   namespace dom{
      typedef unsigned int id;
      enum callbackTypes{};
      struct event{};
      struct callback
      {
         callbackTypes type;
         void (*Callback)(event);
      };


      struct nodeInternal
      {
         id parent;
         std::vector<id> children;
         std::vector<callback> callbacs;
         id tag;
         std::string text;
      };

      class node:public nodeInternal
      {
      private:
         std::shared_ptr<_dom> parentDom;
         id thisId;
         node(std::shared_ptr<_dom> parent,id id);
         void insertNodeRec(const id& parent, const _dom& dom);
         friend _dom;
      public:
         node();
         void insertNode(const node&  toInsert);
      };
   }

   class _dom: public std::enable_shared_from_this<_dom>
   {
   private:
      struct impl;
      impl* pimpl;
      friend dom::node;
   public:
      _dom();
      ~_dom();
      void deleteNode(const dom::id& id);
      void deleteNode(dom::node toDelete);
      std::vector<dom::id> getNodesByTag(const std::string& tag);//TODO implament this
      dom::node getNodeById(dom::id id);
   };
}