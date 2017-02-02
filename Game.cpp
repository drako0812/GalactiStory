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
#include "Game.hpp"

namespace gquest {

    ptr<Game> Game::_instance = nullptr;

    Game::Game() { }
    Game::~Game() {
        auto console = Console::Get();
        console->SetCursorVisible(this->_oldCursorVisible);
        //console->SetPalette(default_palette);
    }
    void Game::Run() {
        auto console = Console::Get();
        console->Initialize();
        console->SetCurrentAttr(Attr::FgWhite);
        console->Resize(GAME_WIDTH, GAME_HEIGHT);
        console->SetTitle(L"GalactiQuest");
        this->_oldCursorVisible = console->CursorVisible();
        console->SetCursorVisible(false);
        console->SetPalette(Softened_Pal);

        this->_subcon1 = std::unique_ptr<SubConsole>(new SubConsole(24, GAME_HEIGHT));
        //this->_playerX = GAME_WIDTH / 2;
        //this->_playerY = GAME_HEIGHT / 2;

        this->_time = 0;
        this->_actionPerformed = false;

        KeyEventHandlerPtr baseKeyEventHandler = KeyEventHandlerPtr(new KeyEventHandler([&](KEY_EVENT_RECORD const& evt) { this->BaseKeyEventHandler(evt); }));
        this->AddKeyEventHandler(baseKeyEventHandler);

        this->_player = uptr<PlayerEntity>(new PlayerEntity(GAME_WIDTH / 2, GAME_HEIGHT / 2));
        this->_entities = { };

        this->_running = true;
        this->Render();
        while(this->_running) {
            this->Update();

            //this->Render();
        }
        HandleExitGameEvent();

        this->RemoveKeyEventHandler(baseKeyEventHandler);
    }
    void Game::Render() {
        auto console = Console::Get();

        this->_subcon1->Clear(L' ', Attr::FgWhite | Attr::BgBlue);
        this->_subcon1->Fill(1, 1, this->_subcon1->Width() - 2, this->_subcon1->Height() - 2, L' ', Attr::FgWhite | Attr::BgBlue);
        this->_subcon1->Box(0, 0, this->_subcon1->Width(), this->_subcon1->Height(), Attr::FgLightCyan | Attr::BgBlue);
        {
            auto p_pos = (components::Position*)(this->_player->GetComponent("Position"_id));
            this->_subcon1->PutString(1, 1, L"T: " + ToString(_time), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
            this->_subcon1->PutString(1, 2, L"X: " + ToString(p_pos->GetPosition().X), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
            this->_subcon1->PutString(1, 3, L"Y: " + ToString(p_pos->GetPosition().Y), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
            this->_subcon1->PutString(1, this->_subcon1->Height() - 2, L"VERSION: " + string(VERSION_STRING), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
        }
        //this->_subcon1->PutString(1, 1, L"X: " + ToString(this->_playerX), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
        //this->_subcon1->PutString(1, 2, L"Y: " + ToString(this->_playerY), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
        console->Clear(L'.', Attr::FgGrey);
        console->Fill(1, 1, GAME_WIDTH - 2, GAME_HEIGHT - 2, L'.', Attr::FgGrey);
        console->Box(this->_subcon1->Width(), 0, GAME_WIDTH - this->_subcon1->Width(), GAME_HEIGHT, Attr::FgLightGrey);
        { // Draw entities
            for(auto entity : _entities) {
                if(
                    entity->HasComponentOfType("Cell"_id) &&
                    entity->HasComponentOfType("Position"_id)
                    ) {

                    auto epos = (components::Position*)(entity->GetComponent("Position"_id));
                    auto ecell = (components::Cell*)(entity->GetComponent("Cell"_id));
                    console->SetChar(epos->GetPosition().X, epos->GetPosition().Y, ecell->GetCChar());
                }
            }
        }
        { // Draw player
            auto p_pos = (components::Position*)(this->_player->GetComponent("Position"_id));
            auto p_cell = (components::Cell*)(this->_player->GetComponent("Cell"_id));
            console->SetChar(p_pos->GetPosition().X, p_pos->GetPosition().Y, p_cell->GetCChar());
        }
        //console->SetChar(this->_playerX, this->_playerY, L'@', Attr::FgLightGreen);
        IConsole::Blit(*console, *this->_subcon1,
            SMALL_RECT{0, 0, this->_subcon1->Width(), this->_subcon1->Height()},
            SMALL_RECT{0, 0, this->_subcon1->Width(), this->_subcon1->Height()});

        console->Display();
    }
    void Game::Update() {
        auto p_controller = (components::PlayerController*)(_player->GetComponent("PlayerController"_id));
        _actionPerformed = false;
        HandleEvents();
        do {
            p_controller->ExecuteCommandsUntil(_time);

            for(auto & entity : _entities) {
                if(entity->HasComponentOfType("LivelySplatterController"_id)) {
                    auto controller = (components::LivelySplatterController*)(entity->GetComponent("LivelySplatterController"_id));
                    controller->ExecuteCommandsUntil(_time);
                }
            }

            Render(); // This was moved here so that when I make commands take more than one tick, each tick can be drawn

            if(_actionPerformed) {
                ++_time;
            }
        } while(!p_controller->CanAct());
    }
    void Game::HandleEvents() {
        auto console = Console::Get();

        INPUT_RECORD evt = console->WaitForEvent();
        do {
            switch(evt.EventType) {
            case KEY_EVENT:
                HandleKeyEvent(evt.Event.KeyEvent);
                break;
            case MOUSE_EVENT:
                HandleMouseEvent(evt.Event.MouseEvent);
                break;
            }
        } while(console->GetEvent(evt));
    }
    void Game::HandleKeyEvent(KEY_EVENT_RECORD const & evt) {
        for(auto const& handler : _keyEventHandlers) {
            (*handler)(evt);
        }
    }
    void Game::HandleMouseEvent(MOUSE_EVENT_RECORD const & evt) {
        for(auto const& handler : _mouseEventHandlers) {
            (*handler)(evt);
        }
    }
    void Game::HandleExitGameEvent() {
        for(auto const& handler : _exitGameEventHandlers) {
            (*handler)();
        }
    }
    void Game::AddKeyEventHandler(KeyEventHandlerPtr keh) {
        auto iter = std::find(std::begin(_keyEventHandlers), std::end(_keyEventHandlers), keh);
        if(iter != std::end(_keyEventHandlers)) { return; }
        _keyEventHandlers.push_back(keh);
    }
    void Game::AddMouseEventHandler(MouseEventHandlerPtr meh) {
        auto iter = std::find(std::begin(_mouseEventHandlers), std::end(_mouseEventHandlers), meh);
        if(iter != std::end(_mouseEventHandlers)) { return; }
        _mouseEventHandlers.push_back(meh);
    }
    void Game::AddExitGameEventHandler(ExitGameEventHandlerPtr xgeh) {
        auto iter = std::find(std::begin(_exitGameEventHandlers), std::end(_exitGameEventHandlers), xgeh);
        if(iter != std::end(_exitGameEventHandlers)) { return; }
        _exitGameEventHandlers.push_back(xgeh);
    }
    void Game::RemoveKeyEventHandler(KeyEventHandlerPtr keh) {
        auto iter = std::find(std::begin(_keyEventHandlers), std::end(_keyEventHandlers), keh);
        if(iter == std::end(_keyEventHandlers)) { return; }
        _keyEventHandlers.erase(iter);
    }
    void Game::RemoveMouseEventHandler(MouseEventHandlerPtr meh) {
        auto iter = std::find(std::begin(_mouseEventHandlers), std::end(_mouseEventHandlers), meh);
        if(iter == std::end(_mouseEventHandlers)) { return; }
        _mouseEventHandlers.erase(iter);
    }
    void Game::RemoveExitGameEventHandler(ExitGameEventHandlerPtr xgeh) {
        auto iter = std::find(std::begin(_exitGameEventHandlers), std::end(_exitGameEventHandlers), xgeh);
        if(iter == std::end(_exitGameEventHandlers)) { return; }
        _exitGameEventHandlers.erase(iter);
    }
    void Game::ClearKeyEventHandlers() {
        _keyEventHandlers.clear();
    }
    void Game::ClearMouseEventHandlers() {
        _mouseEventHandlers.clear();
    }
    void Game::ClearExitGameEventHandlers() {
        _exitGameEventHandlers.clear();
    }

    void Game::BaseKeyEventHandler(KEY_EVENT_RECORD const & evt) {
        switch(evt.wVirtualKeyCode) {
        case VK_ESCAPE:
            if(evt.bKeyDown) {
                this->_running = false;
            }
            break;
        }
    }

    ptr<SubConsole> Game::GetSubConsole1() {
        return this->_subcon1.get();
    }

    uint_ Game::Now() const {
        return _time;
    }

    bool Game::WasActionPerformedThisFrame() const {
        return _actionPerformed;
    }

    void Game::Acted() {
        _actionPerformed = true;
    }

    mt19937_rng & Game::GetRandom() {
        return _rng;
    }

    void Game::AddEntity(EntityPtr entity) {
        auto iter = std::find(std::begin(_entities), std::end(_entities), entity);
        if(iter == std::end(_entities)) {
            _entities.push_back(entity);
            return;
        }
    }

    void Game::RemoveEntity(EntityPtr entity) {
        auto iter = std::find(std::begin(_entities), std::end(_entities), entity);
        if(iter != std::end(_entities)) {
            _entities.erase(iter);
        }
    }

    void Game::RemoveAllEntities() {
        _entities.clear();
    }

    ptr<Game> Game::Get() {
        if(_instance == nullptr) {
            _instance = new Game();
        }
        return _instance;
    }
}
