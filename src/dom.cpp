#include "dom.hpp"

//#include <pybind11/pybind11.h>
//#include <pybind11/embed.h>
//#include <pybind11/eval.h>

#include <vector>
#include <mutex>
#include <map>
#include <unordered_map>
#include <variant>
#include <iostream>

using namespace photon;

namespace photon::globals{
   std::unordered_map<std::string,unsigned int> __tagNames__;
   unsigned int __nextTagId__=1;
}

_dom::_dom()
{
   domObjects[nextid] = {0,{},{},0,""};
   nextid++;
}

_dom::~_dom()
{
}

void _dom::insertNode(const _dom& toinsert)
{
   m.lock();
   dom::id end = toinsert.domObjects.end()->first;
   size_t baseOfset = nextid;
   for(auto& i : toinsert.domObjects)
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

dom::id _dom::insertNode(const dom::nodeInternal& toinsert)
{
   domObjects[toinsert.parent].children.push_back(nextid);
   domObjects[nextid] = toinsert;
   nextid++;
   return nextid-1;
}

void _dom::deleteNode(const dom::id& id)
{
   m.lock();
   deleteNodeRec(id);
   m.unlock();
}

void _dom::deleteNodeRec(const dom::id& id)
{
   for(auto& i : domObjects[id].children)
      deleteNodeRec(i);
   domObjects.erase(id);
}

dom::id _dom::createNode(const dom::id& parent,const std::string& tag, const std::string& data)
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

void _dom::loadFile(std::string path)
{
   // load and parse the html file
}