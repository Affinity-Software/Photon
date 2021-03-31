#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "parser.hpp"

namespace photon{
   class _dom;
   class window;
}

namespace photon::dom
{
   typedef unsigned int id;
   
   enum callbackTypes{};
   
   struct event{};

   struct callback
   {
      callbackTypes type;
      void (*Callback)(event);
   };

   enum _type{
      _node,
      text
   };

   struct nodeInternal
   {
      _type type;
      id parent;
      std::vector<id> children;
      std::vector<callback> callbacs;
      std::map<dom::id,parser::attribute> atributes;
      id tag;
      std::string text;
   };


   class node
   {
   private:
      friend photon::window;
      std::shared_ptr<_dom> parentDom;
      id thisId;
      node(std::shared_ptr<_dom> parent,id id);
   public:
      node();
      void insertNode(const node&  toInsert);
      nodeInternal* operator->();

   };
} // namespace photon::dom
