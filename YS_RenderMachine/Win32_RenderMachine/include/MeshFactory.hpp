#ifndef __YS_MESH_FACTORY_HPP__
#define __YS_MESH_FACTORY_HPP__

#include "Mesh.hpp"
#include "Scene.hpp"


namespace ys_render_machine {


class MeshFactory
{
public:	
	// The purpose of this function is two-fold : 
	// - Create the Mesh structure ready to be eaten by the rendering pipeline.
	// - Create the corresponding Nodes into the Scene.
	static void LoadIntoScene(Scene& _scene, const std::string& _file);

	static void DebugLoadArrays(Scene& _scene, const std::string& _name,
								float* _vertices, int _stride, int _vcount,
								uint32_t* _indices, int _icount);

};


}


#endif // __YS_MESH_FACTORY_HPP__
