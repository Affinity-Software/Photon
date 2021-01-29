#include "photon/dom.hpp"

#include <vector>
#include <mutex>
#include <map>
#include <unordered_map>

using namespace photon;

struct _dom::impl
{
   std::mutex m;
   uint32_t nextid;
   std::map<uint32_t,dom::node> domObjects;
   void deleteNodeRec(const dom::id& id);
   std::unordered_map<std::string,dom::id> tagTypes;

};

_dom::_dom()
{
   pimpl = new _dom::impl();
   pimpl->domObjects[pimpl->nextid] = {0,{},{}};
   pimpl->nextid++;
}

_dom::~_dom()
{
   delete pimpl;
}

void _dom::insertNode(const _dom& toinsert)
{
   pimpl->m.lock();
   dom::id end = toinsert.pimpl->domObjects.end()->first;
   size_t baseOfset = pimpl->nextid;
   for(auto& i : toinsert.pimpl->domObjects)
   {
      dom::node temp = i.second;
      for(auto& j : temp.children)
      {
         j+=baseOfset;
      }
      pimpl->domObjects[i.first+baseOfset] = temp;
   }
   pimpl->nextid += end;
   pimpl->m.unlock();
}

void _dom::insertNode(const dom::node& toinsert)
{
   pimpl->domObjects[toinsert.parent].children.push_back(pimpl->nextid);
   pimpl->domObjects[pimpl->nextid] = toinsert;
   pimpl->nextid++;
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
