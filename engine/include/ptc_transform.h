#ifndef PTC_TRANSFORM_H
#define PTC_TRANSFORM_H

#include <functional>
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
    Object* object;

	Transform(Object* _object);
	Transform(bool _root);
	static Transform* GetRoot();
	void AddChild(Transform* _child);
	void RemoveChild(Transform* _child);
	void PreorderTraversal(std::function<void(Transform*)> processNode);
};

#endif