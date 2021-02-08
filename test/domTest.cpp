#include "photon/window.hpp"

int main(int argc, char const *argv[])
{
   photon::window test;
   auto dom = test.getDom();
   auto node = dom->getNodeById(0);
   photon::dom::node insert;
   photon::dom::node insert2;
   insert.insertNode(insert2);
   node.insertNode(insert);

   return 0;
}
