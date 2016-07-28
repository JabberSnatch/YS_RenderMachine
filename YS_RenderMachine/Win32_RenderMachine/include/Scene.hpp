#ifndef __YS_SCENE_HPP__
#define __YS_SCENE_HPP__

#include "Node.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"


namespace ys_render_machine {


// The Scene class acts pretty much like a resource manager, for now.
// No one except an instance of Scene should free Nodes.
// It also handles the lifetime of Meshes, Shaders and ShaderStages.
class Scene
{
public:
	Scene();
	~Scene();

	Node*				root() const { return m_root; }
	std::vector<Mesh*>&	meshes() { return m_meshes; }

private:
	Node*						m_root;
	std::vector<Mesh*>			m_meshes;
	std::vector<Shader*>		m_shaders;
	std::vector<ShaderStage*>	m_shader_stages;

};


}


#endif // __YS_SCENE_HPP__
