#include "VertexArray.hpp"

#include <cassert>

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
	m_vertex_buffer_size = _source->vertices.size() * sizeof(glm::vec4);
	m_index_count = _source->indices.size();

	glNamedBufferStorage(vbo(), m_vertex_buffer_size,
						 nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(ibo(), index_buffer_size(),
						 nullptr, GL_DYNAMIC_STORAGE_BIT);

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
	//for (size_t i = 0; i < m_bound_mesh->vertices.size(); ++i)
	//{
	//	glNamedBufferSubData(vbo(),
	//						 i * sizeof(float) * 4,
	//						 sizeof(float) * 4,
	//						 m_bound_mesh->vertices[i].E);
	//}
	//
	//for (size_t i = 0; i < m_bound_mesh->indices.size(); ++i)
	//{
	//	glNamedBufferSubData(ibo(),
	//						 i * sizeof(uint32_t),
	//						 sizeof(uint32_t),
	//						 &m_bound_mesh->indices[i]);
	//}

	//uint32_t*	device_ibo = (uint32_t*)glMapNamedBuffer(ibo(), GL_READ_WRITE);
	//memcpy(device_ibo, m_bound_mesh->indices.data(), m_index_buffer_size);
	//assert(glUnmapNamedBuffer(ibo()));

	glNamedBufferSubData(vbo(), 0, m_vertex_buffer_size,
						 m_bound_mesh->vertices.data());
	glNamedBufferSubData(ibo(), 0, index_buffer_size(),
						 m_bound_mesh->indices.data());
	m_model = m_bound_mesh->node->WorldTransform();
}


}
