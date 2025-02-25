#include <algorithm>
#include <ptc_transform.h>

Transform::Transform(Object* _object)
{
	this->position = glm::vec3(0.0f);
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f);
	this->childCount = 0;
	this->children = {};
	this->object = _object;
	if (Transform::GetRoot() == nullptr)
	{
		Transform::SetRoot(this);
		this->parent = nullptr;
	} else
	{
		root->AddChild(this);
	}
}
void Transform::SetRoot(Transform* _root)
{
	root = _root;
}
Transform* Transform::GetRoot()
{
	return root;
}
void Transform::AddChild(Transform* _child)
{
	if (std::find(this->children.begin(), this->children.end(), _child) != this->children.end()) return;
	this->children.push_back(_child);
	this->childCount++;
	_child->parent = this;
}
void Transform::RemoveChild(Transform* _child)
{
	if (std::find(this->children.begin(), this->children.end(), _child) == this->children.end()) return;
	this->children.erase(std::remove(this->children.begin(), this->children.end(), _child), this->children.end());
	this->childCount--;
	_child->parent = nullptr;
}

Transform* Transform::root;