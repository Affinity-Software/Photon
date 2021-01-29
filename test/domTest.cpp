#include "photon/window.hpp"

int main(int argc, char const *argv[])
{
   photon::window test;
   auto dom = test.getDom();

   dom.insertNode({0,{},{}});
   dom.insertNode({0,{},{}});
   dom.insertNode({1,{},{}});
   dom.deleteNode(1);

   return 0;
}
