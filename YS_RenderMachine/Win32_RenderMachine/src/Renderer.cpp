#include "Renderer.hpp"

#include "glew-2.0.0/include/GL/glew.h"


namespace ys_render_machine {


void
Renderer::Render(const RenderData& _render_data)
{
	mat4 projection = mat4::Perspective(0.001f, 1000.f, 60.0f, 16.f / 9.f);
	mat4 view = mat4::Translation(vec4(0.f, 0.f, 100.f));
	mat4 vp_matrix = projection * view;

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(.0f, .0f, .0f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(_render_data.viewport.x, _render_data.viewport.y,
			   _render_data.viewport.width, _render_data.viewport.height);

	_render_data.shader->Enable();
	for (std::vector<VertexArray*>::const_iterator ite = _render_data.meshes.begin();
		 ite != _render_data.meshes.end(); ++ite)
	{
		mat4	mvp_matrix(vp_matrix * (*ite)->model());

		glUniformMatrix4fv(_render_data.shader->UniformLocation("mvp_matrix"),
						   1, GL_FALSE, mvp_matrix.data);
		glBindVertexArray((*ite)->vao());
		glDrawElements(GL_TRIANGLES, (*ite)->index_count(), 
					   GL_UNSIGNED_INT, nullptr);
	}
}


void
Renderer::PrepareRenderData(Scene& _scene, RenderData& o_render_data)
{
	std::vector<VertexArray*>& vertex_arrays = _scene.vertex_arrays();
	for (std::vector<VertexArray*>::iterator ite = vertex_arrays.begin();
		 ite != vertex_arrays.end(); ++ite)
	{
		(*ite)->UpdateBuffers();
		o_render_data.meshes.push_back(*ite);
	}
}


}
