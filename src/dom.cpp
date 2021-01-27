#include "photonGui/dom.hpp"

#include <vector>
#include <mutex>
#include <map>

using namespace photon;

struct _dom::impl
{
   std::mutex m;
   uint32_t nextid;
   std::map<uint32_t,dom::element> domObjects;
};

_dom::_dom()
{
}

_dom::~_dom()
{
}

void _dom::insertElement(const dom::element& toinsert)
{
   pimpl->m.lock();
   pimpl->domObjects[pimpl->nextid] = toinsert;
   pimpl->nextid++;
   pimpl->m.unlock();
}

void _dom::insertElement(const _dom& toinsert)
{
   pimpl->m.lock();
   dom::id start = toinsert.pimpl->domObjects.begin()->first;
   size_t baseOfset = pimpl->nextid;
   for(auto i : toinsert.pimpl->domObjects)
   {
      dom::element temp = i.second;
      for(;;);
      pimpl->domObjects[i.first] = temp;
   }
   pimpl->nextid++;
   pimpl->m.unlock();
}

void _dom::deleteElement(const dom::id& id)
{
   pimpl->m.lock();
   if(pimpl->domObjects[id].children.size())
      for(auto& i : pimpl->domObjects[id].children)
         deleteElement(i);
   pimpl->domObjects.erase(id);
   pimpl->m.unlock();
}
