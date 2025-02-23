#ifndef PTC_COMPONENT_H
#define PTC_COMPONENT_H

#include <ptc_object.h>

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

    void virtual Update() {}
    void virtual FixedUpdate() {}
    void virtual Start() {}
};

#endif