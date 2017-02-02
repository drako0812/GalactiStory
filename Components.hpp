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
#include "Command.hpp"
#include "EventHandler.hpp"

namespace gquest::components {

    class Name : public IComponent {
    private:
        string _name;

    public:
        Name(IEntity * parent=nullptr);
        Name(string const& name, IEntity * parent = nullptr);
        Name(Name const& name);

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
        Position(IEntity * parent = nullptr);
        Position(sysid currentSystem, IVector2 const& systemPosition, bool inSystem, IEntity * parent = nullptr);
        Position(Position const& position);

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

    class Cell : public IComponent {
    private:
        CChar _ch;

    public:
        Cell(IEntity * parent = nullptr);
        Cell(wchar_t ch, Attr attr, IEntity * parent = nullptr);
        Cell(CChar ch, IEntity * parent = nullptr);
        Cell(Cell const& cell);

        CChar GetCChar() const;
        wchar_t GetChar() const;
        Attr GetAttr() const;
        void SetCChar(CChar ch);
        void SetChar(wchar_t ch);
        void SetAttr(Attr attr);

        // Inherited via IComponent
        virtual idtype GetId() const override;

    };

    class IController : public IComponent {
    protected:
        apqueue<Command> _commands;

    public:
        IController(IEntity * parent = nullptr);

        virtual void PushCommand(Command const& command);
        virtual Command const& GetTopCommand() const;
        virtual Command PopCommand();
        virtual bool CommandsAvailable() const;
        virtual void ClearCommands();
        virtual void CancelCommandsSoonerThan(uint_ when);
        virtual void CancelCommandsLaterThan(uint_ when);
        virtual void CancelCommandsAt(uint_ when);
        virtual void ExecuteCommand() = 0;
        virtual void ExecuteCommandsUntil(uint_ when);
    };

    class PlayerController : public IController {
    private:
        KeyEventHandlerPtr _onKeyEvent;
        bool _canAct;
    public:
        PlayerController(IEntity * parent = nullptr);
        ~PlayerController();

        bool CanAct() const;
        void Acted();
        void ClearAct();

        // Inherited via IController
        virtual idtype GetId() const override;
        virtual void ExecuteCommand() override;

    private:
        void onKeyEvent(KEY_EVENT_RECORD const& evt);

        void trySpawnLivelySplatter();
    };

    class LivelySplatterController : public IController {
    public:
        LivelySplatterController(IEntity * parent = nullptr);
        ~LivelySplatterController();

        // Inherited via IController
        virtual idtype GetId() const override;
        virtual void ExecuteCommand() override;

    };

}
