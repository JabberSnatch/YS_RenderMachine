#ifndef __YS_MESH_STORE_HPP__
#define __YS_MESH_STORE_HPP__

#include "Mesh.hpp"

namespace ys_render_machine {


// DESIGN:
// The MeshStore should be able to provide Meshes when needed.
// It manages their lifetime.
class MeshStore
{
public:
	/// Opens the file, reads data, builds the mesh, returns it.
	Mesh* LoadFromFile(const std::string& _id, const std::string& _path);
	/// Builds a whole new mesh using provided array and parameters.
	/// _stride is the number of float representing a vertex.
	/// _vertices size should be at least _stride*_vcount.
	Mesh* LoadFromArrays(const std::string& _id,  
						 float* _vertices, int _stride, int _vcount,
						 uint32_t* _indices, int _icount);

private:
	std::unordered_map<std::string, Mesh*>	m_store;

};


}

#endif // __YS_MESH_STORE_HPP__
