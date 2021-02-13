#include "photon/dom.hpp"

#include <vector>
#include <mutex>
#include <map>
#include <unordered_map>

using namespace photon;

namespace photon::globals{
   std::unordered_map<std::string,unsigned int> __tagNames__;
   unsigned int __nextTagId__=1;
}
struct _dom::impl
{
   std::mutex m;
   uint32_t nextid;
   std::map<uint32_t,dom::nodeInternal> domObjects;
   void deleteNodeRec(const dom::id& id);
   void insertNode(const _dom& toinsert);
   dom::id insertNode(const dom::nodeInternal& toinsert);
   dom::id createNode(const dom::id& parent,const std::string& tag, const std::string& data);
   std::unordered_map<std::string,dom::id> tagTypes;

};

_dom::_dom()
{
   pimpl = new _dom::impl();
   pimpl->domObjects[pimpl->nextid] = {0,{},{},0,""};
   pimpl->nextid++;
}

_dom::~_dom()
{
   delete pimpl;
}

void _dom::impl::insertNode(const _dom& toinsert)
{
   m.lock();
   dom::id end = toinsert.pimpl->domObjects.end()->first;
   size_t baseOfset = nextid;
   for(auto& i : toinsert.pimpl->domObjects)
   {
      dom::nodeInternal temp = i.second;
      for(auto& j : temp.children)
      {
         j+=baseOfset;
      }
      domObjects[i.first+baseOfset] = temp;
   }
   nextid += end;
   m.unlock();
}

dom::id _dom::impl::insertNode(const dom::nodeInternal& toinsert)
{
   domObjects[toinsert.parent].children.push_back(nextid);
   domObjects[nextid] = toinsert;
   nextid++;
   return nextid-1;
}

void _dom::deleteNode(const dom::id& id)
{
   pimpl->m.lock();
   pimpl->deleteNodeRec(id);
   pimpl->m.unlock();
}

void _dom::impl::deleteNodeRec(const dom::id& id)
{
   for(auto& i : domObjects[id].children)
      deleteNodeRec(i);
   domObjects.erase(id);
}

dom::node _dom::getNodeById(dom::id id)
{
   dom::node ret(shared_from_this(),id);
   return ret;
}

dom::node::node(std::shared_ptr<_dom> parent,id id)
{
   thisId = id;
   parentDom = parent;
}

dom::node::node()
{
   parentDom = std::make_shared<_dom>();
   thisId = 0;
}

void dom::node::insertNode(const node& toInsert)
{
   id next = parentDom->pimpl->insertNode({thisId,{},toInsert.callbacs,0,""});
   children.push_back(next);
   for(auto& i : toInsert.children)
   {
      parentDom->pimpl->insertNode({next,{},toInsert.parentDom->pimpl->domObjects[i].callbacs,0,""});
      insertNodeRec(i,*toInsert.parentDom.get());
   }
}

void dom::node::insertNodeRec(const id& parent, const _dom& dom)
{
   for(auto& i : dom.pimpl->domObjects[parent].children)
   {
      auto& temp = dom.pimpl->domObjects[i];
      id nextParent = parentDom->pimpl->insertNode({parent,{},temp.callbacs,0,""});
      if(!temp.children.size())
         insertNodeRec(nextParent,dom);
   }
}

dom::id _dom::impl::createNode(const dom::id& parent,const std::string& tag, const std::string& data)
{
   auto temp = &globals::__tagNames__[tag];
   if(*temp)
   {  
      *temp = globals::__nextTagId__;
      globals::__nextTagId__++;
   }
   domObjects[nextid] = {parent,{},{},*temp,data};
   nextid++;
   return nextid-1;
}