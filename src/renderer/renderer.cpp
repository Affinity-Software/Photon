#include "renderer/renderer.hpp"
#include "photon/style.hpp"
#include <mutex>
#include <array>
#include <tuple>
#include <iterator>
#include <iostream>

namespace photon::renderer
{
	struct vertexData
	{
		glm::vec2 position;
		glm::float32 localCord;
		std::array<glm::vec2,4> rounding;
		std::array<glm::float32,4> borderWidth;
		std::array<glm::vec4,4> borderColor;
		glm::vec4 color;
	};

	static inline glm::vec2 ofsetCordonates(const glm::vec2& point , const glm::vec2& ofset)
	{
		return (point - glm::vec2(1)) * ofset + glm::vec2(1);
	}

	static std::tuple<std::array<vertexData,4>,std::array<unsigned int,6>>
	createElement(const int indexOfset, const glm::vec2& topLeft, const glm::vec2& bottomRight, const photon::style& style)
	{
		std::array<vertexData,4> vertex;

		vertexData templet;
		templet.color = style.bgColor;
		auto border =style.border;
		const glm::vec2 one = {1,1};
		templet.rounding = {
			one / border.bottomLeft,
			one / border.bottomRight,
			one / border.topRight,
			one / border.topLeft
		};
		templet.borderWidth = {
			border.top.width,
			border.right.width,
			border.bottom.width,
			border.left.width
		};

		templet.borderColor = {
			border.top.color,
			border.right.color,
			border.bottom.color,
			border.left.color
		};
	
		for(auto& i:vertex)
			i = templet;

		vertex[0].position = topLeft;
		vertex[1].position = {bottomRight.x,topLeft.y};
		vertex[2].position = bottomRight;
		vertex[3].position = {topLeft.x, bottomRight.y};

		vertex[0].localCord = 0;
		vertex[1].localCord = 1;
		vertex[2].localCord = 2;
		vertex[3].localCord = 3;

		std::array<unsigned int,6> index = {
			0,1,2,
			0,2,3,
		};
		for(auto& i : index)
			i += indexOfset;

		return {vertex,index};
	}

	mesh<
		glm::vec2 ,
		glm::float32 ,
		std::array<glm::vec2,4> ,
		std::array<glm::float32,4> ,
		std::array<glm::vec4,4> ,
		glm::vec4
	>
	domToMesh(const _dom& dom) 
	{
	photon::renderer::mesh<
		glm::vec2 , // 0 0
		glm::float32 , // 1 8
		std::array<glm::vec2,4> , // 2 3 4 5       12 20 28 36
		std::array<glm::float32,4> , // 6 7 8 9    44 48 52 56
		std::array<glm::vec4,4>, // 10 11 12 13    60 76 92 108
		glm::vec4 // 14 124
	> temporary;
		std::vector<vertexData> vertex;
		std::vector<unsigned int> index;
		
		photon::style temp;
		glm::vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
		glm::vec4 grean = {0.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
		glm::vec4 yelow = {1.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 idk = {0.0f, 1.0f, 1.0f, 1.0f};

		temp.border = {
			{0.05f,red},{0.1f,grean},{0.15f,blue},{0.2f,yelow},
			{.3f,.3f},
			{.3f,.3f},
			{.3f,.3f},
			{.3f,.3f}
		};
		temp.bgColor = idk;
		auto [retVertex,retIndex] = createElement(0,{0.8f,0.6f},{-0.8f,-0.6f},temp);

		//std::cout << temporary.strideCheck(sizeof(vertexData)) << std::endl;

		index.insert(index.end(),std::begin(retIndex),std::end(retIndex));
		vertex.insert(vertex.end(),std::begin(retVertex),std::end(retVertex));
		temporary.bind();
		temporary.loadData(vertex,index);
		return temporary;
	}
}