#ifndef PTC_OBJECT_H
#define PTC_OBJECT_H

#include <vector>
#include <memory>
#include <string>

#include <ptc_transform.h>

class Component;

class Object
{
private:
	std::vector<std::unique_ptr<Component>> components;
public:
	std::string name;
	std::vector<std::string> tags;
	Transform transform;
	bool enabled;

	Object(std::string _name = "", bool _enabled = true);

	template <class T>
	T* GetComponent()
	{
		for (auto& component : components) if (T* comp = dynamic_cast<T*>(component.get())) return comp;
		return nullptr;
	}

	std::vector<std::unique_ptr<Component>>* GetComponents();

	template <class T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->SetParentObject(this);
		component->Awake();
		components.push_back(std::move(component));

		return GetComponent<T>();
	}

	static Object* Find(std::string name);
};

#endif