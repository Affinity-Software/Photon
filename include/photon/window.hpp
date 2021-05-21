#pragma once

#include "photon/node.hpp"
#include <filesystem>

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
      void loadHtml(std::filesystem::path path);
   };
}