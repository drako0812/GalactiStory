#pragma once
#include "BaseEntity.hpp"
#include "Components.hpp"

namespace gquest {

    class LivelySplatterEntity :
        public BaseEntity {
    public:

        LivelySplatterEntity() {
            this->AddComponents(
                new components::Name(L"Lively Splatter"),
                new components::Position(0, IVector2(0, 0), false),
                new components::Cell((wchar_t)u'\x2248', Attr::FgLightRed | Attr::BgRed),
                new components::LivelySplatterController()
            );
        }

        LivelySplatterEntity(IVector2 const& pos) {
            this->AddComponents(
                new components::Name(L"Lively Splatter"),
                new components::Position(0, pos, false),
                new components::Cell((wchar_t)u'\x2248', Attr::FgLightRed | Attr::BgRed),
                new components::LivelySplatterController()
            );
        }

        LivelySplatterEntity(int_ x, int_ y) {
            this->AddComponents(
                new components::Name(L"Lively Splatter"),
                new components::Position(0, IVector2(x, y), false),
                new components::Cell((wchar_t)u'\x2248', Attr::FgLightRed | Attr::BgRed),
                new components::LivelySplatterController()
            );
        }

        ~LivelySplatterEntity() {
            this->RemoveAllComponents();
        }
    };

}
