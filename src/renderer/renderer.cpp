#include "renderer/renderer.hpp"
#include <mutex>
#include <array>

namespace photon::renderer
{
    struct vertexData
    {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 localCord;
    };
    mesh<glm::vec2,glm::vec4,glm::vec2> domToMesh(const _dom& dom) 
    {
        photon::renderer::mesh<glm::vec2,glm::vec4,glm::vec2> temporary;
        std::vector<vertexData> vertex;
        vertex.push_back({{-0.75f, -0.75f},{1.0f, 1.0f, 0.0f ,1.0f},{-1.0f, -1.0f}});
        vertex.push_back({{ 0.75f, -0.75f},{1.0f, 1.0f, 0.0f ,1.0f},{-1.0f,  1.0f}});
        vertex.push_back({{ 0.75f,  0.75f},{1.0f, 1.0f, 0.0f ,1.0f},{ 1.0f,  1.0f}});
        vertex.push_back({{-0.75f,  0.75f},{1.0f, 1.0f, 0.0f ,1.0f},{ 1.0f, -1.0f}});
        vertex.push_back({{-0.5f, -0.5f},{1.0f, 1.0f, 1.0f ,1.0f},{-1.0f, -1.0f}});
        vertex.push_back({{ 0.5f, -0.5f},{1.0f, 1.0f, 1.0f ,1.0f},{-1.0f,  1.0f}});
        vertex.push_back({{ 0.5f,  0.5f},{1.0f, 1.0f, 1.0f ,1.0f},{ 1.0f,  1.0f}});
        vertex.push_back({{-0.5f,  0.5f},{1.0f, 1.0f, 1.0f ,1.0f},{ 1.0f, -1.0f}});

        std::vector<unsigned int> index = {
            0,1,2,2,3,0,
            4,5,6,6,7,4
        };

        temporary.bind();
        temporary.loadData(vertex,index);
        return temporary;
    }
}