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

namespace gquest {

    using namespace conlib;
    using namespace randutils;

    constexpr int GAME_WIDTH = 120;
    constexpr int GAME_HEIGHT = 36;

    class Game {
    private:
        std::unique_ptr<SubConsole> _subcon1;
        int _playerX;
        int _playerY;
        bool _running;
        bool _oldCursorVisible;
        mt19937_rng _rng;
    public:
        Game();
        ~Game();
        void Run();
        void Render();
        void Update();
        void HandleEvents();
        void HandleKeyEvent(KEY_EVENT_RECORD const& evt);
    };

}
