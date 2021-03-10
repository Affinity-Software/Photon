#include "renderer/renderer.hpp"
#include "photon/style.hpp"
#include <mutex>
#include <array>
#include <tuple>
#include <iterator>

namespace photon::renderer
{
	struct vertexData
	{
		glm::vec2 position;
		glm::vec4 color;
		glm::vec2 localCord;
		glm::vec4 bordercolor;
	};
	
	static std::tuple<std::array<vertexData,24>,std::array<unsigned int,54>>
	createElement(const int indexOfset, const glm::vec2& topLef, const glm::vec2& bottomRight, const photon::style& style)
	{
		std::array<vertexData,24> vertex;
		/* 
			vertecies
			x = single vertex
			y double vertex
			y----x--------x----y
			|    |        |    |
			x----y--------y----x
			|    |        |    |
			|    |        |    |
			|    |        |    |
			x----y--------y----x
			|    |        |    |
			y----x--------x----y
		 */
		const float xTop[4] = {
			topLef.x,
			topLef.x + style.border.topLeft.x,
			bottomRight.x - style.border.topRight.x,
			bottomRight.x
		};
		const float yLeft[4] = {
			topLef.y,
			topLef.y - style.border.topLeft.y,
			bottomRight.y + style.border.bottomLeft.y,
			bottomRight.y
		};
		const float xBot[4] = {
			topLef.x,
			topLef.x + style.border.bottomLeft.x,
			bottomRight.x - style.border.bottomRight.x,
			bottomRight.x
		};
		const float yRight[4] = {
			topLef.y,
			topLef.y - style.border.topRight.y,
			bottomRight.y + style.border.bottomRight.y,
			bottomRight.y
		};
		vertex[0]  = {{xTop[0], yRight[0]},style.bgColor,{1.f,1.f},style.border.top.color};
		vertex[1]  = {{xTop[1], yRight[0]},style.bgColor,{0.f,1.f},style.border.top.color};
		vertex[2]  = {{xTop[1], yRight[1]},style.bgColor,{0.f,0.f},style.border.top.color};

		vertex[3]  = {{xTop[3], yLeft[0]} ,style.bgColor,{1.f,1.f},style.border.top.color};
		vertex[4]  = {{xTop[2], yLeft[0]} ,style.bgColor,{0.f,1.f},style.border.top.color};
		vertex[5]  = {{xTop[2], yLeft[1]} ,style.bgColor,{0.f,0.f},style.border.top.color};

		vertex[6]  = {{xBot[0], yRight[3]},style.bgColor,{1.f,1.f},style.border.bottom.color};
		vertex[7]  = {{xBot[1], yRight[3]},style.bgColor,{0.f,1.f},style.border.bottom.color};
		vertex[8]  = {{xBot[1], yRight[2]},style.bgColor,{0.f,0.f},style.border.bottom.color};

		vertex[9]  = {{xBot[3], yLeft[3]} ,style.bgColor,{1.f,1.f},style.border.bottom.color};
		vertex[10] = {{xBot[2], yLeft[3]} ,style.bgColor,{0.f,1.f},style.border.bottom.color};
		vertex[11] = {{xBot[2], yLeft[2]} ,style.bgColor,{0.f,0.f},style.border.bottom.color};


		vertex[12] = {{xTop[0], yLeft[0]},style.bgColor,{1.f,1.f},style.border.left.color};
		vertex[13] = {{xTop[0], yLeft[1]},style.bgColor,{1.f,0.f},style.border.left.color};
		vertex[14] = {{xTop[1], yLeft[1]},style.bgColor,{0.f,0.f},style.border.left.color};

		vertex[15] = {{xBot[0], yLeft[2]} ,style.bgColor,{1.f,0.f},style.border.left.color};
		vertex[16] = {{xBot[0], yLeft[3]} ,style.bgColor,{1.f,1.f},style.border.left.color};
		vertex[17] = {{xBot[1], yLeft[2]} ,style.bgColor,{0.f,0.f},style.border.left.color};

		vertex[18] = {{xTop[3], yRight[0]},style.bgColor,{1.f,1.f},style.border.right.color};
		vertex[19] = {{xTop[2], yRight[1]},style.bgColor,{0.f,0.f},style.border.right.color};
		vertex[20] = {{xTop[3], yRight[1]},style.bgColor,{1.f,0.f},style.border.right.color};

		vertex[21] = {{xBot[3], yRight[3]} ,style.bgColor,{1.f,1.f},style.border.right.color};
		vertex[22] = {{xBot[2], yRight[2]} ,style.bgColor,{0.f,0.f},style.border.right.color};
		vertex[23] = {{xBot[3], yRight[2]} ,style.bgColor,{1.f,0.f},style.border.right.color};

		std::array<unsigned int,54> index = {
			//corner tris
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,
			15,16,17,
			18,19,20,
			21,22,23,
			//top quad
			1,2,4,
			4,5,2,
			//bottom quad
			7,8,11,
			10,11,7,
			//left quad
			13,14,15,
			15,17,14,
			//right quad
			19,20,22,
			22,23,20,
			//center quad
			2,5,8,
			5,8,11
		};

		for(int i = 0; i < 54; i++)
			index[i]+=indexOfset;

		return {vertex,index};
	}

	mesh<glm::vec2,glm::vec4,glm::vec2,glm::vec4> domToMesh(const _dom& dom) 
	{
		photon::renderer::mesh<glm::vec2,glm::vec4,glm::vec2,glm::vec4> temporary;
		std::vector<vertexData> vertex;
		std::vector<unsigned int> index;
		/*vertex.push_back({{-0.75f, -0.75f},{1.0f, 1.0f, 0.0f ,1.0f},{-1.0f, -1.0f}});
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
		}; */
		photon::style temp;
		glm::vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
		glm::vec4 grean = {0.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
		glm::vec4 yelow = {1.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 idk = {0.0f, 1.0f, 1.0f, 1.0f};

		temp.border = {
			{0,red},{0,grean},{0,blue},{0,yelow},
			{.2f,.3f},
			{.2f,.3f},
			{.2f,.3f},
			{.2f,.3f}
		};
		temp.bgColor = idk;
		auto [retVertex,retIndex] = createElement(0,{-0.75f,0.75},{0.75f,-0.75f},temp);

		index.insert(index.end(),std::begin(retIndex),std::end(retIndex));
		vertex.insert(vertex.end(),std::begin(retVertex),std::end(retVertex));
		temporary.bind();
		temporary.loadData(vertex,index);
		return temporary;
	}
}