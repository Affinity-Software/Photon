#pragma once

#include "photon/node.hpp"


namespace photon{
   class window
   {
   private:
      struct  impl;
      impl *pimpl;
   public:
      window();
      ~window();
      window(window&) = delete;
      dom::node getRoot();
   };
}