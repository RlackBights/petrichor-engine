#pragma once

#include "Math/Math.h"
#include <functional>
#include <vector>

namespace PetrichorEngine
{
	class Object;
	class Transform
	{
	private:
		static Transform* root;
	public:
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
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
}