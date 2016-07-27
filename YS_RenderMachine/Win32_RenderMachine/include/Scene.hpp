#ifndef __YS_SCENE_HPP__
#define __YS_SCENE_HPP__

#include "Node.hpp"
#include "Mesh.hpp"


namespace ys_render_machine {


// The Scene class acts pretty much like a Node factory, for now.
// No one except an instance of Scene should free Nodes.
// It also handles the lifetime of Meshes.
class Scene
{
public:
	Scene();
	~Scene();

	Node*				root() const { return m_root; }
	std::vector<Mesh*>&	meshes() { return m_meshes; }

private:
	Node*				m_root;
	std::vector<Mesh*>	m_meshes;

};


}


#endif // __YS_SCENE_HPP__
