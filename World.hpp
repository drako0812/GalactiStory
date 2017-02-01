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

#include "GalactiQuestBase.hpp"
#include "IEntity.hpp"

namespace gquest {

    class World {
    private:
        /// <summary>
        /// Holds a mapping of sysids to lists of entities
        /// </summary>
        /// <remarks>
        /// The sysids correspond to the Systems that the entities are
        /// currently in.
        ///
        /// A sysid of 0 indicates that the entity is currently at galactic
        /// level.
        /// </remarks>
        EntityMap _entities;
    public:
        World();

        static constexpr sysid NoSystem = 0;

        /// <summary>
        /// Adds an Entity to the specific system or the galaxy
        /// </summary>
        bool AddEntityToSystem(EntityPtr entity, sysid system_id = NoSystem);

        /// <summary>
        /// Removes an Entity from a system or the galaxy iff it is there
        /// </summary>
        bool RemoveEntityFromSystem(EntityPtr entity, sysid system_id = NoSystem);

        /// <summary>
        /// Removes an Entity from the game world iff it exists
        /// </summary>
        bool RemoveEntity(EntityPtr entity);

        /// <summary>
        /// Moves an Entity to a system or the galaxy
        /// </summary>
        bool MoveEntity(EntityPtr entity, sysid system_id = NoSystem);

        /// <summary>
        /// Moves an Entity to a system or the galaxy iff it is in the specified current system or galaxy
        /// </summary>
        bool MoveEntityFrom(EntityPtr entity, sysid current_system = NoSystem, sysid dest_system = NoSystem);

        using EntityCallback = std::function<void(EntityPtr, sysid)>;
        using EntityPredicate = std::function<bool(EntityPtr, sysid)>;

        /// <summary>
        /// Run a callback on the entity in a particular system or the galaxy iff it is found there
        /// </summary>
        void RunOnEntityInSystem(EntityPtr entity, EntityCallback const& callback, sysid system_id = NoSystem);

        /// <summary>
        /// Run a callback on the entity iff it exists anywhere in the galaxy
        /// </summary>
        void RunOnEntity(EntityPtr entity, EntityCallback const& callback);

        /// <summary>
        /// Run a callback on all the entities in the system or the galaxy
        /// </summary>
        void RunOnEntitiesInSystem(EntityCallback const& callback, sysid system_id = NoSystem);

        /// <summary>
        /// Run a callback on all entities in the game world
        /// </summary>
        void RunOnEntities(EntityCallback const& callback);

        /// <summary>
        /// Run a callback on the entity in the system or the galaxy iff it is found and iff the predicate returns true
        /// </summary>
        void RunOnValidEntityInSystem(EntityPtr entity, EntityPredicate const& predicate, EntityCallback const& callback, sysid system_id = NoSystem);

        /// <summary>
        /// Run a callback on the entity iff it exists in the game world and iff the predicate returns true
        /// </summary>
        void RunOnValidEntity(EntityPtr entity, EntityPredicate const& predicate, EntityCallback const& callback);

        /// <summary>
        /// Run a callback on the entities found in the system or the galaxy iff a particular entity's predicate returns true
        /// </summary>
        void RunOnValidEntitiesInSystem(EntityPredicate const& predicate, EntityCallback const& callback, sysid system_id = NoSystem);

        /// <summary>
        /// Run a callback on all entities in the game world iff a particular entity's predicate returns true
        /// </summary>
        void RunOnValidEntities(EntityPredicate const& predicate, EntityCallback const& callback);

        /// <summary>
        /// Returns true iff the Entity is in the indicated system or at the galaxy level
        /// </summary>
        bool IsEntityInSystem(EntityPtr entity, sysid system_id = NoSystem);

        /// <summary>
        /// Returns true iff the Entity exists anywhere in the galaxy
        /// </summary>
        bool DoesEntityExist(EntityPtr entity);
    };

}
