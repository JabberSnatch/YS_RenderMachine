#ifndef __YS_RENDERER_HPP__
#define __YS_RENDERER_HPP__

#include <vector>
#include "Mesh.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace ys_render_machine {

struct Viewport
{
	int x, y, width, height;
};

class Renderer
{
public:
	struct RenderData
	{
		Viewport					viewport;
		std::vector<VertexArray*>	meshes;
		Shader*						shader;
		
		mat4						view;

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

	// We are starting to have a responsibility problem. It's not the Renderer's
	// job to PrepareRenderData, but we do need someone to update our 
	// VertexArrays.
	static void	Render(const RenderData& _render_data);
	static void PrepareRenderData(Scene& _scene, RenderData& o_render_data);

};


}


#endif // __YS_RENDERER_HPP__
