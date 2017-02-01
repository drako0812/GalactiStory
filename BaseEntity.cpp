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
#include "stdafx.h"
#include "BaseEntity.hpp"

namespace gquest {

    BaseEntity::BaseEntity() : _components() { }

    bool BaseEntity::HasComponentOfType(idtype component_id) const {
        auto iter = _components.find(component_id);
        if(iter != std::end(_components)) {
            return true;
        }
        return false;
    }

    ComponentPtr BaseEntity::GetComponent(idtype component_id) {
        return _components[component_id].get();
    }

    void BaseEntity::AddComponent(ComponentPtr component) {
        _components[component->GetId()] = ComponentUPtr(component);
    }

    void BaseEntity::RemoveComponent(idtype component_id) {
        auto iter = _components.find(component_id);
        if(iter != std::end(_components)) {
            _components.erase(iter);
        }
    }

    void BaseEntity::RemoveAllComponents() {
        _components.clear();
    }

}
