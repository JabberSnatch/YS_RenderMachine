#include "VertexArray.hpp"

#define BUFFER_OFFSET(offset) ((void*)(offset))


namespace ys_render_machine {


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_gl_objects[0]);
	glCreateBuffers(2, &m_gl_objects[1]);
}


VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_gl_objects[0]);
	glDeleteBuffers(2, &m_gl_objects[1]);
}


void
VertexArray::BindMesh(const Mesh* _source)
{
	m_vertex_buffer_size = _source->vertices.size() * sizeof(vec4);
	m_index_buffer_size = _source->indices.size() * sizeof(uint32_t);

	glNamedBufferStorage(vbo(), m_vertex_buffer_size,
						 nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(ibo(), m_index_buffer_size,
						 nullptr, 0);

	glBindVertexArray(vao());
	glBindBuffer(GL_ARRAY_BUFFER, vbo());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo());

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	m_bound_mesh = _source;
}


void
VertexArray::UpdateBuffers()
{
	glNamedBufferSubData(vbo(), 0, m_vertex_buffer_size,
						 m_bound_mesh->vertices.data());
	glNamedBufferSubData(ibo(), 0, m_index_buffer_size,
						 m_bound_mesh->indices.data());
}


}
