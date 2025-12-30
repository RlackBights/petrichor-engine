#pragma once

#include "Object.h"

namespace PetrichorEngine {
    class Object;

    class Component
    {
    public:
        Object* parentObject = nullptr;
        Transform* transform = nullptr;
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

        virtual void Update() {}
        virtual void FixedUpdate() {}
        virtual void Start() {}
        virtual void Awake() {}
    };
}
