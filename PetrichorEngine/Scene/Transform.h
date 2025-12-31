#pragma once

#include "ECS/Object.h"
#include "Math/Math.h"
#include <functional>
#include <vector>

namespace PetrichorEngine::ECS { class Object; }

namespace PetrichorEngine::Scene
{
	class Transform
	{
	private:
		static Transform* root;
	public:
		Math::Vector3 position;
		Math::Quaternion rotation;
		Math::Vector3 scale;
		Transform* parent;
		std::vector<Transform*> children;
		int childCount;
		ECS::Object* object;

		Transform(ECS::Object* _object);
		Transform(bool _root);
		static Transform* GetRoot();
		void AddChild(Transform* _child);
		void RemoveChild(Transform* _child);
		void PreorderTraversal(std::function<void(Transform*)> processNode);
	};
}