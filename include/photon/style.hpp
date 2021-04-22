#pragma once

#include <glm/glm.hpp>
namespace photon
{
   struct style
   {
      struct border
      {
         struct side
         {
            float width;
            glm::vec4 color;
         };
         side top,right,bottom,left;
         glm::vec2 topLeft,bottomLeft,bottomRight,topRight;
      };
      border border;
      enum layout{
         inline_,
         block,
         inlineBlock,
         flex,
         inlineFlex,
         grid,
         inlineGrid
      };

      layout layout;
      struct space
      {
         int top, left, bottom, right;
      };
      space margin;
      space padding;
      glm::vec4 bgColor;
   };
   

} // namespace photon
