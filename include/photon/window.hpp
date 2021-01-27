#pragma once
#include <string>

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
   };
}