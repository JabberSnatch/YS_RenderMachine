#include "Renderer.hpp"

#include "glew-2.0.0/include/GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace ys_render_machine {


void
Renderer::Render(const RenderData& _render_data)
{
	glm::mat4 projection = glm::perspectiveFov(60.f, 1280.f, 720.f, 0.001f, 1000.f);
	glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, -1.f));
	//glm::mat4 view = _render_data.view;
	glm::mat4 vp_matrix = projection * view;

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(.0f, .0f, .0f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(_render_data.viewport.x, _render_data.viewport.y,
			   _render_data.viewport.width, _render_data.viewport.height);

	_render_data.shader->Enable();
	for (std::vector<VertexArray*>::const_iterator ite = _render_data.meshes.begin();
		 ite != _render_data.meshes.end(); ++ite)
	{
		glm::mat4	model = (*ite)->model();
		glm::mat4	mvp_matrix(vp_matrix * model);

		glUniformMatrix4fv(_render_data.shader->UniformLocation("mvp_matrix"),
						   1, GL_TRUE, glm::value_ptr(mvp_matrix));
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
