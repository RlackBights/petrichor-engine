#include <algorithm>
#include <ptc_transform.h>

Transform::Transform()
{
	this->position = glm::vec3(0.0f);
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f);
	this->childCount = 0;
	this->children = {};
	if (!Transform::GetRoot())
	{
		Transform::SetRoot(this);
		this->parent = nullptr;
	} else
	{
		this->parent = root;
	}
}
Transform::Transform(glm::vec3 _position, glm::quat _rotation, glm::vec3 _scale)
{
	this->position = _position;
	this->rotation = _rotation;
	this->scale = _scale;
	this->childCount = 0;
	this->children = {};
	this->parent = root;
}
void Transform::SetRoot(Transform* _root)
{
	root = _root;
}
Transform* Transform::GetRoot()
{
	return root;
}
void Transform::SetParent(Transform* _parent)
{
	this->parent = _parent;
}
void Transform::AddChild(Transform* _child)
{
	if (std::find(this->children.begin(), this->children.end(), _child) != this->children.end()) return;
	this->children.push_back(_child);
	this->childCount++;
	_child->SetParent(this);
}
void Transform::RemoveChild(Transform* _child)
{
	if (std::find(this->children.begin(), this->children.end(), _child) == this->children.end()) return;
	this->children.erase(std::remove(this->children.begin(), this->children.end(), _child), this->children.end());
	this->childCount--;
	_child->SetParent(nullptr);
}

Transform* Transform::root;