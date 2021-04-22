#pragma once

#include "photon/node.hpp"

#include <string>
#include <memory>
#include <mutex>
#include <map>

//TODO proper dom interface
namespace photon
{

   class _dom : public std::enable_shared_from_this<_dom>
   {
   private:
      std::mutex m;
      uint32_t nextid = 0;
      void deleteNodeRec(const dom::id &id);
      friend dom::node;

   public:
      std::map<uint32_t, dom::nodeInternal> domObjects;
      _dom();
      ~_dom();
      void deleteNode(const dom::id &id);
      void deleteNode(dom::node toDelete);
      void insertNode(const _dom &toinsert);
      dom::id insertNode(const dom::nodeInternal &toinsert);
      dom::id createNode(const dom::id &parent, const std::string &tag,const std::map<std::string, std::string>& attributes);
      dom::id crateTextNode(const dom::id &parent, const std::string &text);
      std::vector<dom::id> getNodesByTag(const std::string &tag); //TODO implament this
      void loadFile(std::string path);
   };
}