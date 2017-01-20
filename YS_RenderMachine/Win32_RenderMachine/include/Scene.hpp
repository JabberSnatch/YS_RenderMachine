#ifndef __YS_SCENE_HPP__
#define __YS_SCENE_HPP__

#include "Node.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"


namespace ys_render_machine {


// The Scene class acts pretty much like a resource manager, for now.
// No one except an instance of Scene should free Nodes.
// It also handles the lifetime of Meshes, Shaders, ShaderStages, VertexArrays,
// Textures.
class Scene
{
public:
	Scene();
	~Scene();

	Node*						root() const { return m_root; }
	std::vector<Mesh*>&			meshes() { return m_meshes; }
	std::vector<VertexArray*>&	vertex_arrays() { return m_vertex_arrays; }
	std::vector<Texture*>&		textures() { return m_textures; }

private:
	Node*						m_root;
	std::vector<Mesh*>			m_meshes;
	std::vector<VertexArray*>	m_vertex_arrays;
	std::vector<Shader*>		m_shaders;
	std::vector<ShaderStage*>	m_shader_stages;
	std::vector<Texture*>		m_textures;

};


}


#endif // __YS_SCENE_HPP__
