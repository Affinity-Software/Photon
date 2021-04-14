#include "photon/window.hpp"
#include "photon/parser.hpp"
#include <iostream>

int main()
{
   photon::window test;
   auto root = test.getRoot();
   photon::dom::node insert;
   photon::dom::node insert2;
   insert.insertNode(insert2);
   root.insertNode(insert);
}