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
#include "IComponent.hpp"
#include "Vector2.hpp"

namespace gquest::components {

    class Name : public IComponent {
    private:
        string _name;

    public:
        Name();
        Name(string const& name);

        string const& GetName();
        void SetName(string const& name);

        // Inherited via IComponent
        virtual idtype GetId() const override;
    };

    class Position : public IComponent {
    private:
        sysid _currentSystem;
        IVector2 _currentSystemPosition;
        bool _inSystem;

    public:
        Position();
        Position(sysid currentSystem, IVector2 const& systemPosition, bool inSystem);

        sysid GetCurrentSystem() const;
        IVector2 const& GetPosition() const;
        bool IsInSystem() const;
        void SetCurrentSystem(sysid system);
        void SetPosition(IVector2 const& position);
        void SetPosition(int_ x, int_ y);
        void SetIsInSystem(bool inSystem);

        // Inherited via IComponent
        virtual idtype GetId() const override;
    };

}
