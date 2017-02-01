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
#include "Components.hpp"

namespace gquest::components {

    idtype Name::GetId() const {
        return L"Name"_id;
    }

    Name::Name() : _name() { }

    Name::Name(string const & name) : _name(name) { }

    Name::Name(Name const & name) : _name(name._name) { }

    string const & Name::GetName() {
        return _name;
    }

    void Name::SetName(string const & name) {
        _name = name;
    }

    
    Position::Position() : _currentSystem(0), _currentSystemPosition(), _inSystem(false) { }

    Position::Position(sysid currentSystem, IVector2 const & systemPosition, bool inSystem) :
        _currentSystem(currentSystem), _currentSystemPosition(systemPosition), _inSystem(inSystem) { }

    sysid Position::GetCurrentSystem() const {
        return _currentSystem;
    }

    IVector2 const & Position::GetPosition() const {
        return _currentSystemPosition;
    }

    bool Position::IsInSystem() const {
        return _inSystem;
    }

    void Position::SetCurrentSystem(sysid system) {
        _currentSystem = system;
    }

    void Position::SetPosition(IVector2 const & position) {
        _currentSystemPosition = position;
    }

    void Position::SetPosition(int_ x, int_ y) {
        _currentSystemPosition.X = x;
        _currentSystemPosition.Y = y;
    }

    void Position::SetIsInSystem(bool inSystem) {
        _inSystem = inSystem;
    }

    idtype Position::GetId() const {
        return "Position"_id;
    }

}
