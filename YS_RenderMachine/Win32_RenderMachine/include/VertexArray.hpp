#ifndef __YS_VERTEX_ARRAY_HPP__
#define __YS_VERTEX_ARRAY_HPP__

#include "glew-2.0.0/include/GL/glew.h"
#include "Mesh.hpp"


namespace ys_render_machine {


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void	BindMesh(const Mesh* _source);
	void	UpdateBuffers();

	GLuint		vao() const { return m_gl_objects[0]; }
	GLuint		vbo() const { return m_gl_objects[1]; }
	GLuint		ibo() const { return m_gl_objects[2]; }

	const mat4&	model() const { return m_model; }

	size_t		vertex_buffer_size() const { return m_vertex_buffer_size; }
	size_t		index_buffer_size() const { return m_index_buffer_size; }

private:
	const Mesh*	m_bound_mesh;
	mat4		m_model;
	GLuint		m_gl_objects[3];

	size_t		m_vertex_buffer_size;
	size_t		m_index_buffer_size;

};


}


#endif // __YS_VERTEX_ARRAY_HPP__
