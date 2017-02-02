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
#include "Game.hpp"
#include "IEntity.hpp"

namespace gquest::components {

    idtype Name::GetId() const {
        return L"Name"_id;
    }

    Name::Name(IEntity * parent) : IComponent(parent), _name() { }

    Name::Name(string const & name, IEntity * parent) : IComponent(parent), _name(name) { }

    Name::Name(Name const & name) : IComponent(name._parent), _name(name._name) { }

    string const & Name::GetName() {
        return _name;
    }

    void Name::SetName(string const & name) {
        _name = name;
    }


    Position::Position(IEntity * parent) : IComponent(parent), _currentSystem(0), _currentSystemPosition(), _inSystem(false) { }

    Position::Position(sysid currentSystem, IVector2 const & systemPosition, bool inSystem, IEntity * parent) :
        IComponent(parent), _currentSystem(currentSystem), _currentSystemPosition(systemPosition), _inSystem(inSystem) { }

    Position::Position(Position const& position) :
        IComponent(position._parent),
        _currentSystem(position._currentSystem),
        _currentSystemPosition(position._currentSystemPosition),
        _inSystem(position._inSystem) { }

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

    idtype Cell::GetId() const {
        return "Cell"_id;
    }

    Cell::Cell(IEntity * parent) : IComponent(parent), _ch{L' ', Attr::FgWhite} { }

    Cell::Cell(wchar_t ch, Attr attr, IEntity * parent) : IComponent(parent), _ch{ch, attr} { }

    Cell::Cell(CChar ch, IEntity * parent) : IComponent(parent), _ch(ch) { }

    Cell::Cell(Cell const & cell) : IComponent(cell._parent), _ch(cell._ch) { }

    CChar Cell::GetCChar() const {
        return _ch;
    }

    wchar_t Cell::GetChar() const {
        return _ch.Char.UnicodeChar;
    }

    Attr Cell::GetAttr() const {
        return (Attr)_ch.Attributes;
    }

    void Cell::SetCChar(CChar ch) {
        _ch = ch;
    }

    void Cell::SetChar(wchar_t ch) {
        _ch.Char.UnicodeChar = ch;
    }

    void Cell::SetAttr(Attr attr) {
        _ch.Attributes = attr;
    }

    IController::IController(IEntity * parent) : IComponent(parent) { }

    void IController::PushCommand(Command const & command) {
        _commands.push(command);
    }

    Command const & IController::GetTopCommand() const {
        return _commands.top();
    }

    Command IController::PopCommand() {
        auto cmd = _commands.top();
        _commands.pop();
        return cmd;
    }

    bool IController::CommandsAvailable() const {
        return !_commands.empty();
    }

    void IController::ClearCommands() {
        while(!_commands.empty()) {
            _commands.pop();
        }
    }

    void IController::CancelCommandsSoonerThan(uint_ when) {
        while(!_commands.empty()) {
            if(_commands.top().GetWhen() < when) {
                _commands.pop();
            } else {
                break;
            }
        }
    }

    void IController::CancelCommandsLaterThan(uint_ when) {
        apqueue<Command> tmpq;
        while(!_commands.empty()) {
            if(_commands.top().GetWhen() > when) {
                break;
            } else {
                tmpq.push(_commands.top());
                _commands.pop();
            }
        }
        _commands = tmpq;
    }

    void IController::CancelCommandsAt(uint_ when) {
        apqueue<Command> tmpq;
        while(!_commands.empty()) {
            if(_commands.top().GetWhen() != when) {
                tmpq.push(_commands.top());
                _commands.pop();
            } else {
                _commands.pop();
            }
        }
        _commands = tmpq;
    }

    void IController::ExecuteCommandsUntil(uint_ when) {
        while(CommandsAvailable()) {
            auto command = GetTopCommand();
            if(command.GetWhen() <= when) {
                ExecuteCommand();
            } else {
                return;
            }
        }
    }

    PlayerController::PlayerController(IEntity * parent) : IController(parent) {
        auto game = Game::Get();
        _onKeyEvent = KeyEventHandlerPtr(
            new KeyEventHandler(
                [&](KEY_EVENT_RECORD const& evt) {onKeyEvent(evt); }
            )
        );
        game->AddKeyEventHandler(_onKeyEvent);
        _canAct = true;
    }

    PlayerController::~PlayerController() {
        auto game = Game::Get();
        game->RemoveKeyEventHandler(_onKeyEvent);
    }

    bool PlayerController::CanAct() const {
        return _canAct;
    }

    void PlayerController::Acted() {
        _canAct = false;
    }

    void PlayerController::ClearAct() {
        _canAct = true;
    }

    idtype PlayerController::GetId() const {
        return "PlayerController"_id;
    }

    void PlayerController::ExecuteCommand() {
        auto command = _commands.top();
        switch(command.GetCommandType()) {
        case CommandType::MoveDown:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if(posr.Y + 1 < Console::Get()->Height() - 1) {
                //PlaySoundW(L"data\\sfx_move_001.wav", nullptr, SND_FILENAME);
                pos->SetPosition(
                    posr.X,
                    posr.Y + 1
                );
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveUp:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if(posr.Y - 1 > 0) {
                //PlaySoundW(L"data\\sfx_move_001.wav", nullptr, SND_FILENAME);
                pos->SetPosition(
                    posr.X,
                    posr.Y - 1
                );
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveRight:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if(posr.X + 1 < Console::Get()->Width() - 1) {
                //PlaySoundW(L"data\\sfx_move_001.wav", nullptr, SND_FILENAME);
                pos->SetPosition(
                    posr.X + 1,
                    posr.Y
                );
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveLeft:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if(posr.X - 1 > Game::Get()->GetSubConsole1()->Width()) {
                //PlaySoundW(L"data\\sfx_move_001.wav", nullptr, SND_FILENAME);
                pos->SetPosition(
                    posr.X - 1,
                    posr.Y
                );
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveLeftUp:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if((posr.X - 1 > Game::Get()->GetSubConsole1()->Width()) &&
                (posr.Y - 1 > 0)) {

                pos->SetPosition(posr.X - 1, posr.Y - 1);
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveRightUp:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if((posr.X + 1 < Console::Get()->Width() - 1) &&
                (posr.Y - 1 > 0)) {

                pos->SetPosition(posr.X + 1, posr.Y - 1);
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveLeftDown:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if((posr.X - 1 > Game::Get()->GetSubConsole1()->Width()) &&
                (posr.Y + 1 < Console::Get()->Height() - 1)) {

                pos->SetPosition(posr.X - 1, posr.Y + 1);
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::MoveRightDown:
        {
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            if((posr.X + 1 < Console::Get()->Width() - 1) &&
                (posr.Y + 1 < Console::Get()->Height() - 1)) {

                pos->SetPosition(posr.X + 1, posr.Y + 1);
            }
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::Wait:
            //PlaySoundW(L"SystemStart", nullptr, SND_ALIAS);
            ClearAct();
            PopCommand();
            break;
        case CommandType::DEBUG_BecomeSmiley:
        {
            auto cell = (components::Cell*)(_parent->GetComponent("Cell"_id));
            cell->SetChar((wchar_t)u'\x263b');
            cell->SetAttr(Attr::FgLightRed);
            PopCommand();
            break;
        }
        case CommandType::DEBUG_BecomePlayer:
        {
            auto cell = (components::Cell*)(_parent->GetComponent("Cell"_id));
            cell->SetChar(L'@');
            cell->SetAttr(Attr::FgLightGreen);
            ClearAct();
            PopCommand();
            break;
        }
        case CommandType::LivelySplatter_Spawn:
        {
            auto args = command.GetArguments();
            int_ X, Y;
            auto iter = std::begin(args);
            (iter++)->get(X);
            iter->get(Y);
            Game::Get()->AddEntity(
                EntityPtr(
                    new LivelySplatterEntity(X, Y)
                )
            );
            PopCommand();
            break;
        }
        }
        
    }

    void PlayerController::onKeyEvent(KEY_EVENT_RECORD const & evt) {
        if(!CanAct()) {
            return;
        }
        if(evt.bKeyDown) {
            switch(evt.wVirtualKeyCode) {
            case VK_DOWN:
            case VK_NUMPAD2:
                this->PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::MoveDown
                    )
                );
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_UP:
            case VK_NUMPAD8:
                this->PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::MoveUp
                    )
                );
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_RIGHT:
            case VK_NUMPAD6:
                this->PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::MoveRight
                    )
                );
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_LEFT:
            case VK_NUMPAD4:
                this->PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::MoveLeft
                    )
                );
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_DECIMAL:
            case VK_NUMPAD5:
                this->PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::Wait
                    )
                );
                Acted();
                Game::Get()->Acted();
                break;
            case VK_NUMPAD7:
                this->PushCommand(Command(Game::Get()->Now() + 1, CommandType::MoveLeftUp));
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_NUMPAD9:
                this->PushCommand(Command(Game::Get()->Now() + 1, CommandType::MoveRightUp));
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_NUMPAD1:
                this->PushCommand(Command(Game::Get()->Now() + 1, CommandType::MoveLeftDown));
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case VK_NUMPAD3:
                this->PushCommand(Command(Game::Get()->Now() + 1, CommandType::MoveRightDown));
                trySpawnLivelySplatter();
                Acted();
                Game::Get()->Acted();
                break;
            case 'Q':
                this->PushCommand(Command(Game::Get()->Now(), CommandType::DEBUG_BecomeSmiley));
                this->PushCommand(Command(Game::Get()->Now() + 19, CommandType::DEBUG_BecomePlayer));
                Acted();
                Game::Get()->Acted();
                break;
            case 'W':
            {
                auto pos = (components::Position*)this->GetParent()->GetComponent("Position"_id);
                PushCommand(
                    Command(
                        Game::Get()->Now(),
                        CommandType::LivelySplatter_Spawn,
                        pos->GetPosition().X,
                        pos->GetPosition().Y
                    )
                );
                //Acted();
                Game::Get()->Acted();
                break;
            }
            default:
                if(evt.uChar.UnicodeChar == L'.') {
                    this->PushCommand(
                        Command(
                            Game::Get()->Now(),
                            CommandType::Wait
                        )
                    );
                    Acted();
                    Game::Get()->Acted();
                }
                break;
            }
        }
    }

    void PlayerController::trySpawnLivelySplatter() {
        const vec<bool> choices = {0,0,0,1};
        if(Game::Get()->GetRandom().pick(choices)) {
            auto pos = (components::Position*)this->GetParent()->GetComponent("Position"_id);
            PushCommand(
                Command(
                    Game::Get()->Now(),
                    CommandType::LivelySplatter_Spawn,
                    pos->GetPosition().X,
                    pos->GetPosition().Y
                )
            );
        }
    }

    LivelySplatterController::LivelySplatterController(IEntity * parent) : IController(parent) {
        PushCommand(
            Command(
                Game::Get()->Now() + Game::Get()->GetRandom().uniform(1, 10),
                CommandType::LivelySplatter_Move
            )
        );
    }

    LivelySplatterController::~LivelySplatterController() { }

    idtype LivelySplatterController::GetId() const {
        return "LivelySplatterController"_id;
    }

    void LivelySplatterController::ExecuteCommand() {
        auto command = _commands.top();
        switch(command.GetCommandType()) {
        case CommandType::LivelySplatter_Move:
        {
            const ui64vec dirs = {0, 1, 2, 3};
            auto game = Game::Get();
            auto pos = (components::Position*)(_parent->GetComponent("Position"_id));
            auto posr = pos->GetPosition();
            uint_ direction = game->GetRandom().pick(dirs);
            switch(direction) {
            case 0: // Left
                if(posr.X - 1 > Game::Get()->GetSubConsole1()->Width()) {
                    pos->SetPosition(
                        posr.X - 1,
                        posr.Y
                    );
                }
                break;
            case 1: // Up
                if(posr.Y - 1 > 0) {
                    pos->SetPosition(
                        posr.X,
                        posr.Y - 1
                    );
                }
                break;
            case 2: // Right
                if(posr.X + 1 < Console::Get()->Width() - 1) {
                    pos->SetPosition(
                        posr.X + 1,
                        posr.Y
                    );
                }
                break;
            case 3: // Down
                if(posr.Y + 1 < Console::Get()->Height() - 1) {
                    pos->SetPosition(
                        posr.X,
                        posr.Y + 1
                    );
                }
                break;
            }

            PopCommand();

            // Cause another move in the future
            PushCommand(
                Command(
                    Game::Get()->Now() + Game::Get()->GetRandom().uniform(1, 10),
                    CommandType::LivelySplatter_Move
                )
            );

            break;
        }
        }
        
    }

}
