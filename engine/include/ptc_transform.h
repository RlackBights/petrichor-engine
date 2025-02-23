#ifndef PTC_TRANSFORM_H
#define PTC_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Object;

class Transform
{
private:
	static Transform* root;
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	Transform* parent;
	std::vector<Transform*> children;
	int childCount;
    Object* parentObject = nullptr;

	Transform();
	Transform(glm::vec3 _position, glm::quat _rotation, glm::vec3 _scale);
	static void SetRoot(Transform* _root);
	static Transform* GetRoot();
	void SetParent(Transform* _parent);
	void AddChild(Transform* _child);
	void RemoveChild(Transform* _child);
};

#endif