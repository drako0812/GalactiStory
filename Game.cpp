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
    Game::Game() { }
    Game::~Game() {
        auto console = Console::Get();
        console->SetCursorVisible(this->_oldCursorVisible);
        console->SetPalette(default_palette);
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
        this->_playerX = GAME_WIDTH / 2;
        this->_playerY = GAME_HEIGHT / 2;

        this->_running = true;
        this->Render();
        while(this->_running) {
            this->Update();

            this->Render();
        }
    }
    void Game::Render() {
        auto console = Console::Get();

        this->_subcon1->Clear(L' ', Attr::FgWhite | Attr::BgBlue);
        this->_subcon1->Fill(1, 1, this->_subcon1->Width() - 2, this->_subcon1->Height() - 2, L' ', Attr::FgWhite | Attr::BgBlue);
        this->_subcon1->Box(0, 0, this->_subcon1->Width(), this->_subcon1->Height(), Attr::FgLightCyan | Attr::BgBlue);
        this->_subcon1->PutString(1, 1, L"X: " + ToString(this->_playerX), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
        this->_subcon1->PutString(1, 2, L"Y: " + ToString(this->_playerY), Attr::FgWhite | Attr::BgBlue, this->_subcon1->Width() - 2);
        console->Clear(L'.', Attr::FgGrey);
        console->Fill(1, 1, GAME_WIDTH - 2, GAME_HEIGHT - 2, L'.', Attr::FgGrey);
        console->Box(this->_subcon1->Width(), 0, GAME_WIDTH - this->_subcon1->Width(), GAME_HEIGHT, Attr::FgLightGrey);
        console->SetChar(this->_playerX, this->_playerY, L'@', Attr::FgLightGreen);
        IConsole::Blit(*console, *this->_subcon1,
            SMALL_RECT{0, 0, this->_subcon1->Width(), this->_subcon1->Height()},
            SMALL_RECT{0, 0, this->_subcon1->Width(), this->_subcon1->Height()});

        console->Display();
    }
    void Game::Update() {
        HandleEvents();
    }
    void Game::HandleEvents() {
        auto console = Console::Get();

        INPUT_RECORD evt = console->WaitForEvent();
        do {
            switch(evt.EventType) {
            case KEY_EVENT:
                HandleKeyEvent(evt.Event.KeyEvent);
                break;
            }
        } while(console->GetEvent(evt));
    }
    void Game::HandleKeyEvent(KEY_EVENT_RECORD const & evt) {
        switch(evt.wVirtualKeyCode) {
        case VK_UP:
            if(evt.bKeyDown) {
                if((this->_playerY - 1) > 0) {
                    this->_playerY--;
                }
            }
            break;
        case VK_DOWN:
            if(evt.bKeyDown) {
                if((this->_playerY + 1) <= GAME_HEIGHT - 2) {
                    this->_playerY++;
                }
            }
            break;
        case VK_LEFT:
            if(evt.bKeyDown) {
                if((this->_playerX - 1) > this->_subcon1->Width()) {
                    this->_playerX--;
                }
            }
            break;
        case VK_RIGHT:
            if(evt.bKeyDown) {
                if((this->_playerX + 1) <= GAME_WIDTH - 2) {
                    this->_playerX++;
                }
            }
            break;
        case VK_ESCAPE:
            if(evt.bKeyDown) {
                this->_running = false;
            }
            break;
        }
    }
}
