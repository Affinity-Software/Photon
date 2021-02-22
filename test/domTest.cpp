#include "photon/window.hpp"

int main(int argc, char const *argv[])
{
   photon::window test;
   auto root = test.getRoot();
   photon::dom::node insert;
   photon::dom::node insert2;
   insert.insertNode(insert2);
   root.insertNode(insert);

   return 0;
}
