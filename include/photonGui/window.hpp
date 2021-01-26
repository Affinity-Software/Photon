#pragma once
#include <string>

namespace photonGui{
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