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
		glm::vec4 color;
		glm::vec2 localCord;
		glm::vec2 topLefRounding;
		glm::vec2 topRightRounding;
		glm::vec2 bottomLefRounding;
		glm::vec2 bottomRightRounding;
		glm::vec4 bordercolor;
	};

	static inline glm::vec2 ofsetCordonates(const glm::vec2& point , const glm::vec2& ofset)
	{
		return (point - glm::vec2(1)) * ofset + glm::vec2(1);
	}

	static std::tuple<std::array<vertexData,14>,std::array<unsigned int,18>>
	createElement(const int indexOfset, const glm::vec2& topLeft, const glm::vec2& bottomRight, const photon::style& style)
	{
		std::array<vertexData,14> vertex;

		/* 
			vertex structure
			x--------------x
			| \          / |
			|  x--------x  |
			| /          \ |
			x--------------x
		 */

		const float with = topLeft.x - bottomRight.x;
		const float height = topLeft.y - bottomRight.y;
		float lessSignificantDirecrioPerTwo;

		if(with > height)
			lessSignificantDirecrioPerTwo = height/2;
		else
			lessSignificantDirecrioPerTwo = with/2;

		const glm::vec2 positions[6] = {
			{topLeft                 },
			{bottomRight.x, topLeft.y},
			{bottomRight             },
			{topLeft.x, bottomRight.y},
			{topLeft - glm::vec2(lessSignificantDirecrioPerTwo)},
			{bottomRight + glm::vec2(lessSignificantDirecrioPerTwo)}
		};

		//style rounding values
		// t = top, b = bottom, l = left, r = right, r = roundind
		const glm::vec2  tlr = glm::vec2(1) / style.border.topLeft;
		const glm::vec2  trr = glm::vec2(1) / style.border.topRight;
		const glm::vec2  blr = glm::vec2(1) / style.border.bottomLeft;
		const glm::vec2  brr = glm::vec2(1) / style.border.bottomRight;

		//iner vertex local cordinates
		const glm::vec2 tl = {
			with > height ? ((height/with)/2) : .5f,
			with > height ? .5f : ((with/height)/2)
		};
		const glm::vec2 br = {
			with > height ? 1 - ((height/with)/2) : .5f,
			with > height ? .5f : 1 - ((with/height)/2)
		};

		//rounding fields
		const glm::vec2 roundtl[6] = {
			ofsetCordonates({1,1},tlr),
			ofsetCordonates({0,1},blr),
			ofsetCordonates({0,0},blr),
			ofsetCordonates({1,0},blr),
			ofsetCordonates(br,blr),
			ofsetCordonates(tl,blr)
		};
		const glm::vec2 roundtr[6] = {
			ofsetCordonates({0,1},trr),
			ofsetCordonates({1,1},brr),
			ofsetCordonates({1,0},brr),
			ofsetCordonates({0,0},brr),
			ofsetCordonates(tl,brr),
			ofsetCordonates(br,brr)
		};
		const glm::vec2 roundbl[6] = {
			ofsetCordonates({1,0},blr),
			ofsetCordonates({0,0},trr),
			ofsetCordonates({0,1},trr),
			ofsetCordonates({1,1},trr),
			ofsetCordonates(br,trr),
			ofsetCordonates(tl,trr)
		};
		const glm::vec2 roundbr[6] = {
			ofsetCordonates({0,0},brr),
			ofsetCordonates({1,0},tlr),
			ofsetCordonates({1,1},tlr),
			ofsetCordonates({0,1},tlr),
			ofsetCordonates(tl,tlr),
			ofsetCordonates(br,tlr)
		};

		if(with > height)
		{
			vertex[0] = {positions[0],style.bgColor,{1,1},roundtl[0],roundtr[0],roundbr[0],roundbl[0],style.border.top.color};
			vertex[1] = {positions[1],style.bgColor,{1,1},roundtl[1],roundtr[1],roundbr[1],roundbl[1],style.border.top.color};
			vertex[2] = {positions[4],style.bgColor,{0,0},roundtl[4],roundtr[4],roundbr[4],roundbl[4],style.border.top.color};
			vertex[3] = {positions[5],style.bgColor,{0,0},roundtl[5],roundtr[5],roundbr[5],roundbl[5],style.border.top.color};

			vertex[4] = {positions[2],style.bgColor,{1,1},roundtl[2],roundtr[2],roundbr[2],roundbl[2],style.border.bottom.color};
			vertex[5] = {positions[3],style.bgColor,{1,1},roundtl[3],roundtr[3],roundbr[3],roundbl[3],style.border.bottom.color};
			vertex[6] = {positions[4],style.bgColor,{0,0},roundtl[4],roundtr[4],roundbr[4],roundbl[4],style.border.bottom.color};
			vertex[7] = {positions[5],style.bgColor,{0,0},roundtl[5],roundtr[5],roundbr[5],roundbl[5],style.border.bottom.color};

			vertex[8] =  {positions[0],style.bgColor,{1,1},roundtl[0],roundtr[0],roundbr[0],roundbl[0],style.border.left.color};
			vertex[9] =  {positions[3],style.bgColor,{1,1},roundtl[3],roundtr[3],roundbr[3],roundbl[3],style.border.left.color};
			vertex[10] = {positions[4],style.bgColor,{0,0},roundtl[4],roundtr[4],roundbr[4],roundbl[4],style.border.left.color};

			vertex[11] = {positions[1],style.bgColor,{1,1},roundtl[1],roundtr[1],roundbr[1],roundbl[1],style.border.right.color};
			vertex[12] = {positions[2],style.bgColor,{1,1},roundtl[2],roundtr[2],roundbr[2],roundbl[2],style.border.right.color};
			vertex[13] = {positions[5],style.bgColor,{0,0},roundtl[5],roundtr[5],roundbr[5],roundbl[5],style.border.right.color};
		}
		else
		{
			std::cerr << "not implemented" << std::endl;
		}

		std::array<unsigned int,18> index = {
			0,1,2,
			1,2,3,
			4,5,6,
			4,6,7,
			8,9,10,
			11,12,13
		};
		for(int i = 0; i < 54; i++)
			index[i]+=indexOfset;

		return {vertex,index};
	}

	mesh<glm::vec2,glm::vec4,glm::vec2,glm::vec2,glm::vec2,glm::vec2,glm::vec2,glm::vec4> domToMesh(const _dom& dom) 
	{
		photon::renderer::mesh<glm::vec2,glm::vec4,glm::vec2,glm::vec2,glm::vec2,glm::vec2,glm::vec2,glm::vec4> temporary;
		std::vector<vertexData> vertex;
		std::vector<unsigned int> index;
		
		photon::style temp;
		glm::vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
		glm::vec4 grean = {0.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
		glm::vec4 yelow = {1.0f, 1.0f, 0.0f, 1.0f};
		glm::vec4 idk = {0.0f, 1.0f, 1.0f, 1.0f};

		temp.border = {
			{0,red},{0,grean},{0,blue},{0,yelow},
			{.8f,.3f},
			{.2f,.3f},
			{.2f,.3f},
			{.2f,.3f}
		};
		temp.bgColor = idk;
		auto [retVertex,retIndex] = createElement(0,{0.8f,0.6f},{-0.8f,-0.6f},temp);

		index.insert(index.end(),std::begin(retIndex),std::end(retIndex));
		vertex.insert(vertex.end(),std::begin(retVertex),std::end(retVertex));
		temporary.bind();
		temporary.loadData(vertex,index);
		return temporary;
	}
}