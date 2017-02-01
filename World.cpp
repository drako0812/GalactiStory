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
#include "World.hpp"

namespace gquest {

    World::World() {
        _entities[0] = EntityList{ };
    }

    bool World::AddEntityToSystem(EntityPtr entity, sysid system_id) {
        if(_entities.find(system_id) == std::end(_entities)) {
            _entities[system_id] = EntityList{ };
        }
        if(std::find(std::begin(_entities[system_id]), std::end(_entities[system_id]), entity) != std::end(_entities[system_id])) {
            return false;
        }
        _entities[system_id].push_back(entity);
        return true;
    }

    bool World::RemoveEntityFromSystem(EntityPtr entity, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return false;
        }
        auto ent_iter = std::find(std::begin(iter->second), std::end(iter->second), entity);
        if(ent_iter == std::end(iter->second)) {
            return false;
        }
        _entities[system_id].erase(ent_iter);
        return true;
    }

    bool World::RemoveEntity(EntityPtr entity) {
        for(auto & system : _entities) {
            auto ent_iter = std::find(std::begin(system.second), std::end(system.second), entity);
            if(ent_iter != std::end(system.second)) {
                system.second.erase(ent_iter);
                return true;
            }
        }
        return false;
    }

    bool World::MoveEntity(EntityPtr entity, sysid system_id) {
        if(RemoveEntity(entity)) {
            return AddEntityToSystem(entity, system_id);
        }
        return false;
    }

    bool World::MoveEntityFrom(EntityPtr entity, sysid current_system, sysid dest_system) {
        if(RemoveEntityFromSystem(entity, current_system)) {
            return AddEntityToSystem(entity, dest_system);
        }
        return false;
    }

    void World::RunOnEntityInSystem(EntityPtr entity, EntityCallback const & callback, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return;
        }
        auto ent_iter = std::find(std::begin(iter->second), std::end(iter->second), entity);
        if(ent_iter == std::end(iter->second)) {
            return;
        }
        callback(entity, system_id);
    }

    void World::RunOnEntity(EntityPtr entity, EntityCallback const & callback) {
        for(auto & system : _entities) {
            auto ent_iter = std::find(std::begin(system.second), std::end(system.second), entity);
            if(ent_iter != std::end(system.second)) {
                callback(entity, system.first);
                return;
            }
        }
    }

    void World::RunOnEntitiesInSystem(EntityCallback const & callback, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return;
        }
        for(auto & entity : iter->second) {
            callback(entity, system_id);
        }
    }

    void World::RunOnEntities(EntityCallback const & callback) {
        for(auto & system : _entities) {
            for(auto & entity : system.second) {
                callback(entity, system.first);
            }
        }
    }

    void World::RunOnValidEntityInSystem(EntityPtr entity, EntityPredicate const & predicate, EntityCallback const & callback, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return;
        }
        auto ent_iter = std::find(std::begin(iter->second), std::end(iter->second), entity);
        if(ent_iter == std::end(iter->second)) {
            return;
        }
        if(predicate(entity, system_id)) {
            callback(entity, system_id);
        }
    }

    void World::RunOnValidEntity(EntityPtr entity, EntityPredicate const & predicate, EntityCallback const & callback) {
        for(auto & system : _entities) {
            auto ent_iter = std::find(std::begin(system.second), std::end(system.second), entity);
            if(ent_iter != std::end(system.second)) {
                if(predicate(entity, system.first)) {
                    callback(entity, system.first);
                }
                return;
            }
        }
    }

    void World::RunOnValidEntitiesInSystem(EntityPredicate const & predicate, EntityCallback const & callback, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return;
        }
        for(auto & entity : iter->second) {
            if(predicate(entity, system_id)) {
                callback(entity, system_id);
            }
        }
    }

    void World::RunOnValidEntities(EntityPredicate const & predicate, EntityCallback const & callback) {
        for(auto & system : _entities) {
            for(auto & entity : system.second) {
                if(predicate(entity, system.first)) {
                    callback(entity, system.first);
                }
            }
        }
    }

    bool World::IsEntityInSystem(EntityPtr entity, sysid system_id) {
        auto iter = _entities.find(system_id);
        if(iter == std::end(_entities)) {
            return false;
        }
        auto ent_iter = std::find(std::begin(iter->second), std::end(iter->second), entity);
        if(ent_iter == std::end(iter->second)) {
            return false;
        }
        return true;
    }

    bool World::DoesEntityExist(EntityPtr entity) {
        for(auto const& system : _entities) {
            auto ent_iter = std::find(std::begin(system.second), std::end(system.second), entity);
            if(ent_iter != std::end(system.second)) {
                return true;
            }
        }
        return false;
    }

}
