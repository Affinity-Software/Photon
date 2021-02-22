#include "photon/node.hpp"
#include "dom.hpp"

using namespace photon::dom;

nodeInternal* node::operator->()
{
   return &parentDom->domObjects[thisId];
}

node::node(std::shared_ptr<_dom> parent,id id)
{
   thisId = id;
   parentDom = parent;
}

node::node()
{
   parentDom = std::make_shared<_dom>();
   thisId = 0;
}

static void insertNodeRec(id parent, id start, std::shared_ptr<photon::_dom> targer, std::shared_ptr<photon::_dom> source)
{
   nodeInternal temp = source->domObjects[start];
   temp.children = {};
   temp.parent = parent;
   id newParent = targer->insertNode(temp);
   for(auto& i : source->domObjects[start].children)
      insertNodeRec(newParent,i,targer,source);
}

void node::insertNode(const node& toInsert)
{
   insertNodeRec(thisId,toInsert.thisId,parentDom,toInsert.parentDom);
}