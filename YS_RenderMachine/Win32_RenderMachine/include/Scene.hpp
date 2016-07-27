#ifndef __YS_SCENE_HPP__
#define __YS_SCENE_HPP__

#include "Node.hpp"


namespace ys_render_machine {


// The Scene class acts pretty much like a Node factory, for now.
// No one except an instance of Scene should free Nodes.
class Scene
{
public:
	Scene();
	~Scene();

	Node*		root() const { return m_root; }

private:
	Node*		m_root;

};


}


#endif // __YS_SCENE_HPP__
