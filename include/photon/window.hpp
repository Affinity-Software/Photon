#pragma once
#include <string>
#include <memory>
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
      std::shared_ptr<_dom> getDom();
   };
}