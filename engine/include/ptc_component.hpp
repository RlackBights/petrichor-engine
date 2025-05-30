#ifndef PTC_COMPONENT_HPP
#define PTC_COMPONENT_HPP

#include "ptc_object.hpp"

class Object;

class Component
{
public:
    Object* parentObject = nullptr;
    bool enabled = true;
    virtual ~Component() = default;

    Object* GetParentObject();
    void SetParentObject(Object* object);

    template <class T>
    T* GetComponent()
    {
        return parentObject->GetComponent<T>();
    }

    template <class T, typename... Args>
	T* AddComponent(Args&&... args)
	{
        return parentObject->AddComponent<T>(args...);
	}

    void virtual Update() {}
    void virtual FixedUpdate() {}
    void virtual Start() {}
    void virtual Awake() {}
};

#endif