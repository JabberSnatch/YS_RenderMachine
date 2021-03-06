#ifndef __YS_NODE_HPP__
#define __YS_NODE_HPP__

#include <vector>
#include <string>
#include "glm/matrix.hpp"


namespace ys_render_machine {


// BUGS ARE EXPECTED ON THIS.
// BE WARY OF CIRCULAR REFERENCE DON'T F*CK THIS UP MAN.

// Node represents a Node in the Scene's graph. 
// It stores a mat4 representing its transformation in 3D space.
// Even though a Scene is usually represented as a tree structure,
// Node doesn't take sides and allows the user to produce cycles in the graph.
// However, a Node can only have a single parent, which means that a Node will
// not appear in more than one children vector.
class Node
{
public:
	Node() 
		: m_name(""), m_parent(nullptr), m_index_in_parent(0), m_transform(1.f)
	{}
	Node(const std::string& _name)
		: m_name(_name), m_parent(nullptr), m_index_in_parent(0), m_transform(1.f)
	{}

	// SetParent calls remove_child on the Node current parent, 
	// and AddChild on the new one.
	// It handles getting a nullptr or setting the node as its own parent. (Should it?)
	void				SetParent(Node* _parent);
	// AddChild request the new child to unregister from its current parent.
	// Then it adds it to the caller's children and updates 
	// _child m_index_index_in_parent and m_parent.
	void				AddChild(Node* _child);

	// WorldTransform runs through the Node's hierarchy and computes the transform
	// of the Node relative to the identity transform.
	glm::mat4			WorldTransform();

	bool				HasParent() const { return m_parent != nullptr; }
	bool				HasChildren() const { return !m_children.empty(); }
	// NOTE: Add a function to check for cycles.
	// bool				HasCycle() const;

	void				SetName(const std::string& _name) { m_name = _name; }

	const std::string&	name() const { return m_name; }
	Node*				parent() const { return m_parent; }
	std::vector<Node*>&	children() { return m_children; }
	glm::mat4&			transform() { return m_transform; }
	
private:
	// remove_child does the dirty work of SetParent. It pushes the child
	// back to m_children's last position and pops it.
	// It also updates the removed child's parent to be nullptr.
	void				remove_child(unsigned int index);

	std::string			m_name;

	Node*				m_parent;
	unsigned int		m_index_in_parent;

	std::vector<Node*>	m_children;

	/// The transform of the node relative to its parent.
	/// If it has no parent it is relative to the canonical frame.
	glm::mat4			m_transform;

};


}


#endif // __YS_NODE_HPP__
