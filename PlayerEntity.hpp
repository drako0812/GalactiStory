#pragma once
#include "BaseEntity.hpp"
#include "Components.hpp"

namespace gquest {

    class PlayerEntity :
        public BaseEntity {
    public:

        PlayerEntity() {
            this->AddComponents(
                new components::Name(L"Player"),
                new components::Position(0, IVector2(0, 0), false),
                new components::Cell(L'@', Attr::FgLightGreen),
                new components::PlayerController()
            );
        }

        PlayerEntity(IVector2 const& pos) {
            this->AddComponents(
                new components::Name(L"Player"),
                new components::Position(0, pos, false),
                new components::Cell(L'@', Attr::FgLightGreen),
                new components::PlayerController()
            );
        }

        PlayerEntity(int_ x, int_ y) {
            this->AddComponents(
                new components::Name(L"Player"),
                new components::Position(0, IVector2(x, y), false),
                new components::Cell(L'@', Attr::FgLightGreen),
                new components::PlayerController()
            );
        }

        ~PlayerEntity() {
            this->RemoveAllComponents();
        }
    };

}
