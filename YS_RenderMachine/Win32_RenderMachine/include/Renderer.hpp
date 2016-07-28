#ifndef __YS_RENDERER_HPP__
#define __YS_RENDERER_HPP__

#include "glew-2.0.0/include/GL/glew.h"
#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"

namespace ys_render_machine {


class Renderer
{
public:
	struct RenderData
	{
		std::vector<Mesh*>	meshes;
		
		// We don't really want the mesh's vertex data here.
		// All we care about is each mesh VAO so we should probably provide
		// them in lighter structure along with model matrices.
		
		// Also we need a shader for each mesh.
		// It could be something like pair<Shader, list<mesh index>>
		// This way we could iterate over our shaders and avoid switching 
		// shader on every draw call.

		// Also also MVP matrix should be computed on the CPU, we don't need 
		// to compute it for each vertex, once for each mesh is enough.
	};

	static void	Render(const RenderData& _render_data);

};


}


#endif // __YS_RENDERER_HPP__
