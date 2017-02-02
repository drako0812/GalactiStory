// The MIT License (MIT)
//
// Copyright (c) 2017 Drew Wibbenmeyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include "IEntity.hpp"

namespace gquest {

    class BaseEntity : public IEntity {
    protected:
        ComponentMap _components;
    public:
        BaseEntity();
        
        template<class ...ComponentTypes>
        BaseEntity(ComponentTypes ...components);

        template<class ...ComponentTypes>
        void AddComponents(ComponentTypes ...components);

        template<class ...IdTypes>
        void RemoveComponents(IdTypes ...components);

        // Inherited via IEntity
        virtual bool HasComponentOfType(idtype component_id) const override;
        virtual ComponentPtr GetComponent(idtype component_id) override;
        virtual void AddComponent(ComponentPtr component) override;
        virtual void RemoveComponent(idtype component_id) override;

    protected:
        template<class ComponentType, class ...ComponentTypes>
        void addComponents(ComponentType component, ComponentTypes ...components);

        template<class ComponentType>
        void addComponents(ComponentType component);

        template<class IdType, class ...IdTypes>
        void removeComponents(IdType component, IdTypes ...components);

        template<class IdType>
        void removeComponents(IdType component);

        // Inherited via IEntity
        virtual void RemoveAllComponents() override;
    };

    template<class ...ComponentTypes>
    inline BaseEntity::BaseEntity(ComponentTypes ...components) : BaseEntity() {
        addComponents(components...);
    }

    template<class ...ComponentTypes>
    inline void BaseEntity::AddComponents(ComponentTypes ...components) {
        addComponents(components...);
    }

    template<class ...IdTypes>
    inline void BaseEntity::RemoveComponents(IdTypes ...components) {
        removeComponents(components...);
    }

    template<class ComponentType, class ...ComponentTypes>
    inline void BaseEntity::addComponents(ComponentType component, ComponentTypes ...components) {
        AddComponent(component);
        addComponents(components...);
    }

    template<class ComponentType>
    inline void BaseEntity::addComponents(ComponentType component) {
        AddComponent(component);
    }

    template<class IdType, class ...IdTypes>
    inline void BaseEntity::removeComponents(IdType component, IdTypes ...components) {
        RemoveComponent(component);
        removeComponents(components...);
    }

    template<class IdType>
    inline void BaseEntity::removeComponents(IdType component) {
        RemoveComponent(component);
    }

}
