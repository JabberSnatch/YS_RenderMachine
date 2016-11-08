#ifndef __YS_MESH_HPP__
#define __YS_MESH_HPP__

#include <unordered_map>
#include <string>
#include "glm/vec4.hpp"
#include "Node.hpp"


namespace ys_render_machine {


// NOTE: If we compute animations on a separate stage, the renderer should not
//		 even care about bones.
// A Mesh stores a fixed number of vertices and bones.
// It shouldn't change over time. The protocol here is to create a new Mesh
// every time we want to compute an animation state or something.
// Eventually it will also store UVs and Normal space.
struct Mesh
{
	// A bone stores the transform it corresponds to, and a map of every vertex
	// it affects.
	struct Bone
	{
		Node*								node;
		std::unordered_map<uint64_t, float>	weight_map;
	};

	Node*					node;
	std::vector<glm::vec4>	vertices;
	std::vector<uint32_t>	indices;
	std::unordered_map<std::string, Bone>	bones;

	glm::vec4				bounds[2];
};


glm::mat4 ComputeDebugView(const Mesh& _mesh, float _fov);


}


#endif // __YS_MESH_HPP__
