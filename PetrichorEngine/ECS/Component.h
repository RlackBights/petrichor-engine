#pragma once

#include "Object.h"
#include "Scene/Transform.h"

namespace PetrichorEngine::ECS {
    class Object;

    class Component
    {
    public:
        Object* parentObject = nullptr;
        Scene::Transform* transform = nullptr;
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
