#include "Node.hpp"

#include <cassert>


namespace ys_render_machine {


// NOTE: The responsibility is oddly split with SetParent, AddChild and remove_child.
//		 For instance SetParent doesn't change the node's parent at all.
void
Node::SetParent(Node* _parent)
{
	if (m_parent != _parent)
	{
		if (m_parent)
			m_parent->remove_child(m_index_in_parent);

		if (_parent)
			_parent->AddChild(this);
	}
}


void
Node::AddChild(Node* _child)
{
	if (_child->m_parent != this)
	{
		if (_child->m_parent)
			_child->SetParent(nullptr);

		m_children.push_back(_child);
		_child->m_index_in_parent = unsigned int(m_children.size() - 1);
		_child->m_parent = this;
	}
}


void
Node::remove_child(unsigned int index)
{
	assert(index < m_children.size());

	m_children[index]->m_parent = nullptr;
	for (int i = index + 1; i < m_children.size(); ++i)
	{
		m_children[i]->m_index_in_parent--;
		std::swap(m_children[i - 1], m_children[i]);
	}

	m_children.pop_back();
}


glm::mat4
Node::WorldTransform()
{
	glm::mat4	result;
	Node*		cursor = this;

	while (cursor->HasParent())
	{
		result = cursor->transform() * result;
		cursor = cursor->parent();
	}

	return result;
}


}

