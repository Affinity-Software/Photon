#pragma once
#include <string>
#include "photon/dom.hpp"

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
      _dom& getDom();
   };
}