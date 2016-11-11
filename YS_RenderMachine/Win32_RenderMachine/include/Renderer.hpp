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
		
		glm::mat4					view;
		glm::mat4					projection;

		// We need a shader for each mesh.
		// It could be something like pair<Shader, list<mesh index>>
		// This way we could iterate over our shaders and avoid switching 
		// shader on every draw call.
	};

	static void	Render(const RenderData& _render_data);

};


}


#endif // __YS_RENDERER_HPP__
