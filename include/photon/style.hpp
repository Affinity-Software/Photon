namespace photon
{
   struct style
   {
      struct border
      {
         struct side
         {
            int width;
            int color;
         };
         side top,right,bottom,left;
         struct radius
         {
            int x,y;
         };
         radius topLeft,bottomLeft,bottomRight,topRight;
      };
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
      space margin,padding;
   };
   

} // namespace photon
