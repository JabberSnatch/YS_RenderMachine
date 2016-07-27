#ifndef __YS_MESH_HPP__
#define __YS_MESH_HPP__

#include <unordered_map>
#include <string>
#include "mat4.hpp"


namespace ys_render_machine {


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
		mat4*								transform;
		std::unordered_map<uint64_t, float>	weight_map;
	};

	mat4*				transform;
	std::vector<vec4>	vertices;
	std::unordered_map<std::string, Bone>	bones;

};


}


#endif // __YS_MESH_HPP__
