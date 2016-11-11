#include "Renderer.hpp"

#include "glew-2.0.0/include/GL/glew.h"
#include "glm/gtc/type_ptr.hpp"


namespace ys_render_machine {


void
Renderer::Render(const RenderData& _render_data)
{
	glm::mat4 vp_matrix = _render_data.projection * _render_data.view;

	// GL STATE
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.0f, .0f, .0f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(_render_data.viewport.x, _render_data.viewport.y,
			   _render_data.viewport.width, _render_data.viewport.height);

	// DRAW
	_render_data.shader->Enable();
	for (std::vector<VertexArray*>::const_iterator ite = _render_data.meshes.begin();
		 ite != _render_data.meshes.end(); ++ite)
	{
		glm::mat4	model = (*ite)->model();
		glm::mat4	mvp_matrix(vp_matrix * model);

		glUniformMatrix4fv(_render_data.shader->UniformLocation("mvp_matrix"),
						   1, GL_FALSE, glm::value_ptr(mvp_matrix));
		//glUniformMatrix4fv(_render_data.shader->UniformLocation("model"),
		//				   1, GL_FALSE, (*ite)->model().data);
		//glUniformMatrix4fv(_render_data.shader->UniformLocation("view"),
		//				   1, GL_FALSE, view.data);
		//glUniformMatrix4fv(_render_data.shader->UniformLocation("projection"),
		//				   1, GL_FALSE, projection.data);
		glBindVertexArray((*ite)->vao());
		glDrawElements(GL_TRIANGLES, (*ite)->index_count(), 
					   GL_UNSIGNED_INT, nullptr);
	}
}


}
