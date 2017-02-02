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

#include "randutils.hpp"
#include "ConLibBase.hpp"
#include "Console.hpp"
#include "SubConsole.hpp"
#include "GalactiQuestBase.hpp"
#include "EventHandler.hpp"
#include "PlayerEntity.hpp"
#include "LivelySplatterEntity.hpp"

namespace gquest {

    using namespace conlib;
    using namespace randutils;

    constexpr int GAME_WIDTH = 120;
    constexpr int GAME_HEIGHT = 36;

    class Game {
    private:
        uptr<SubConsole> _subcon1;
        uptr<PlayerEntity> _player;
        vec<EntityPtr> _entities;
        //int _playerX;
        //int _playerY;
        bool _running;
        bool _oldCursorVisible;
        bool _actionPerformed;
        uint_ _time;
        mt19937_rng _rng;

        vec<KeyEventHandlerPtr> _keyEventHandlers;
        vec<MouseEventHandlerPtr> _mouseEventHandlers;
        vec<ExitGameEventHandlerPtr> _exitGameEventHandlers;

        static ptr<Game> _instance;
    public:
        Game();
        ~Game();
        void Run();
        void Render();
        void Update();
        void HandleEvents();
        void HandleKeyEvent(KEY_EVENT_RECORD const& evt);
        void HandleMouseEvent(MOUSE_EVENT_RECORD const& evt);
        void HandleExitGameEvent();

        void AddKeyEventHandler(KeyEventHandlerPtr keh);
        void AddMouseEventHandler(MouseEventHandlerPtr meh);
        void AddExitGameEventHandler(ExitGameEventHandlerPtr xgeh);

        void RemoveKeyEventHandler(KeyEventHandlerPtr keh);
        void RemoveMouseEventHandler(MouseEventHandlerPtr meh);
        void RemoveExitGameEventHandler(ExitGameEventHandlerPtr xgeh);

        void ClearKeyEventHandlers();
        void ClearMouseEventHandlers();
        void ClearExitGameEventHandlers();

        void BaseKeyEventHandler(KEY_EVENT_RECORD const& evt);

        ptr<SubConsole> GetSubConsole1();

        uint_ Now() const;
        bool WasActionPerformedThisFrame() const;
        void Acted();

        mt19937_rng & GetRandom();

        void AddEntity(EntityPtr entity);
        void RemoveEntity(EntityPtr entity);
        void RemoveAllEntities();

        static ptr<Game> Get();
    };

}
