#include "renderer/renderer.hpp"
#include <mutex>
#include <array>

namespace photon::renderer
{
    mesh<glm::vec2> domToMesh(const _dom& dom) 
    {
        photon::renderer::mesh<glm::vec2> temporary;
        auto vertex = new glm::vec2[4];
        vertex[0] = {-0.5, -0.5};
        vertex[1] = { 0.5, -0.5};
        vertex[2] = { 0.5,  0.5};
        vertex[3] = {-0.5,  0.5};

        auto index = new unsigned int[6];
        
        index[0] = 0;
        index[1] = 1;
        index[2] = 2;
        index[3] = 2;
        index[4] = 3;
        index[5] = 0;

        vertexBufferData temp = {vertex,4,index,6};
        temporary.bind();
        temporary.loadData(temp);
        return temporary;
    }
}