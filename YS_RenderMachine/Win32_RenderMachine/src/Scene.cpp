#include "Scene.hpp"

#include <vector>


namespace ys_render_machine {


Scene::Scene()
{
	m_root = new Node("ROOT_NODE");
}


Scene::~Scene()
{
	std::vector<Node*>	nodes_by_depth;
	Node*				current_node;
	
	nodes_by_depth.push_back(m_root);

	while (nodes_by_depth.size())
	{
		current_node = nodes_by_depth.back();

		if (current_node->HasChildren())
		{
			std::vector<Node*>&		children = current_node->children();

			for (std::vector<Node*>::iterator ite = children.begin();
				 ite != children.end();
				 ++ite)
			{
				nodes_by_depth.push_back(*ite);
			}
		}
		else
		{
			nodes_by_depth.pop_back();
			current_node->SetParent(nullptr);
			delete current_node;
		}
	}

	for (std::vector<Mesh*>::iterator ite = m_meshes.begin();
		 ite != m_meshes.end(); ++ite)
	{
		delete *ite;
	}

	for (std::vector<Shader*>::iterator ite = m_shaders.begin();
		 ite != m_shaders.end(); ++ite)
	{
		delete *ite;
	}

	for (std::vector<ShaderStage*>::iterator ite = m_shader_stages.begin();
		 ite != m_shader_stages.end(); ++ite)
	{
		delete *ite;
	}

	for (std::vector<VertexArray*>::iterator ite = m_vertex_arrays.begin();
		 ite != m_vertex_arrays.end(); ++ite)
	{
		delete *ite;
	}

	for (std::vector<Texture*>::iterator ite = m_textures.begin();
		 ite != m_textures.end(); ++ite)
	{
		delete *ite;
	}
}


}

