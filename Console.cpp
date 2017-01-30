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
#include "Console.hpp"

namespace conlib {

    Console::Console() {
        _init = false;
        _width = -1;
        _height = -1;
        _oldWinWidth = -1;
        _oldWinHeight = -1;
        _curAttr = Attr::None;
        _buffer.clear();
        _backBuf.clear();
        //_dirtyBuf.clear();
    }

    Console::~Console() {
        _buffer.clear();
        _backBuf.clear();
        //_dirtyBuf.clear();
        if(_init) {
            SetPalette(_oldPalette);

            auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD c;
            c.X = _oldBufWidth;
            c.Y = _oldBufHeight;
            SetConsoleScreenBufferSize(hOut, c);
            SMALL_RECT sr;
            sr.Left = 0;
            sr.Right = _oldWinWidth;
            sr.Top = 0;
            sr.Bottom = _oldWinHeight;
            SetConsoleWindowInfo(hOut, true, &sr);
            SetConsoleTextAttribute(hOut, _oldAttr);

            auto hIn = GetStdHandle(STD_INPUT_HANDLE);

            SetConsoleMode(hOut, _oldOutMode);
            SetConsoleMode(hIn, _oldInMode);

            SetConsoleOutputCP(_oldOutputCodePage);
            SetConsoleCP(_oldInputCodePage);

            _init = false;
        }
    }

    std::unique_ptr<Console> Console::_instance = nullptr;

    Console * Console::Get() {
        if(_instance == nullptr) {
            _instance = std::unique_ptr<Console>{new Console()};
        }
        return _instance.get();
    }

    void Console::Initialize() {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        _oldInputCodePage = GetConsoleCP();
        _oldOutputCodePage = GetConsoleOutputCP();
        bool cp_set_success = SetConsoleCP(CP_UTF8) == TRUE;
        cp_set_success = SetConsoleOutputCP(CP_UTF8) == TRUE;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        _oldBufWidth = csbi.dwSize.X;
        _oldBufHeight = csbi.dwSize.Y;
        _oldAttr = (Attr)csbi.wAttributes;
        _oldWinWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        _oldWinHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        _curAttr = _oldAttr;
        _width = _oldWinWidth;
        _height = _oldWinHeight;
        resizeConsole(_width, _height);
        resizeBuf(_width, _height);

        auto hIn = GetStdHandle(STD_INPUT_HANDLE);

        GetConsoleMode(hOut, &_oldOutMode);
        GetConsoleMode(hIn, &_oldInMode);
        SetConsoleMode(hOut, 0);
        SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

        GetPalette(_oldPalette);

        SetAllDirty();

        //_dirty = true;
        _init = true;
    }

    CChar Console::GetChar(short x, short y) const {
        return _backBuf[x + y * _width];
    }

    short Console::Width() const {
        return _width;
    }

    short Console::Height() const {
        return _height;
    }

    void Console::SetWidth(short width) {
        Resize(width, _height);
    }

    void Console::SetHeight(short height) {
        Resize(_width, height);
    }

    void Console::Size(short & width, short & height) const {
        width = _width;
        height = _height;
    }

    void Console::Resize(short width, short height) {
        if((width != _width) || (height != _height)) {
            //_dirty = true;
            _width = width;
            _height = height;
            resizeConsole(width, height);
            resizeBuf(width, height);
            SetAllDirty();

            Display();
        }
    }

    Attr Console::CurrentAttr() const {
        return _curAttr;
    }

    void Console::SetCurrentAttr(Attr attr) {
        _curAttr = attr;
    }

    void Console::SetChar(short x, short y, CChar ch) {
        if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) {
            return;
        }
        if(!Equal(_backBuf[x + y * _width], ch)) {
            _backBuf[x + y * _width] = ch;
            //_dirty = true;
            //_dirtyBuf[x + y * _width] = true;
        }
    }

    void Console::SetChar(short x, short y, wchar_t ch) {
        SetChar(x, y, CChar{ch, _curAttr});
    }

    void Console::SetChar(short x, short y, wchar_t ch, Attr attr) {
        SetChar(x, y, CChar{ch, attr});
    }

    void Console::PutString(short x, short y, std::wstring const & str, Attr attr, int max_length) {
        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.length()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.length()) && (i < max_length) && (i + start_idx < _backBuf.size()); ++i) {
            tmpch.Char.UnicodeChar = str[i];
            tmpch.Attributes = attr;
            if(!Equal(_backBuf[i + start_idx], tmpch)) {
                _backBuf[i + start_idx] = tmpch;
                //_dirty = true;
                //_dirtyBuf[i + start_idx] = true;
            }
        }
    }

    void Console::PutString(short x, short y, std::wstring const & str, int max_length) {
        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.length()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.length()) && (i < max_length) && (i + start_idx < _backBuf.size()); ++i) {
            tmpch.Char.UnicodeChar = str[i];
            tmpch.Attributes = _curAttr;
            if(!Equal(_backBuf[i + start_idx], tmpch)) {
                _backBuf[i + start_idx] = tmpch;
                //_dirty = true;
                //_dirtyBuf[i + start_idx] = true;
            }
        }
    }

    void Console::Clear() {
        CChar ch;
        ch.Char.UnicodeChar = L' ';
        ch.Attributes = _curAttr;
        Clear(ch);
        //memset(_buffer.data(), *((int*)(&ch)), _buffer.size());
        //_dirty = true;
    }

    void Console::Clear(wchar_t fill_ch) {
        CChar ch;
        ch.Char.UnicodeChar = fill_ch;
        ch.Attributes = _curAttr;
        Clear(ch);
        //memset(_buffer.data(), *((int*)(&ch)), _buffer.size());
        //_dirty = true;
    }

    void Console::Clear(CChar fill_ch) {
        for(int y = 0; y < _height; ++y) {
            for(int x = 0; x < _width; ++x) {
                SetChar(x, y, fill_ch);
            }
        }
    }

    void Console::Clear(wchar_t fill_ch, Attr attr) {
        CChar ch;
        ch.Char.UnicodeChar = fill_ch;
        ch.Attributes = attr;
        Clear(ch);
    }

    void Console::Box(short x, short y, short width, short height, BoxType box_type) {
        switch(box_type) {
        case BoxType::Ascii:
            Box(x, y, width, height, _curAttr, L'+', L'-', L'+', L'|', L'|', L'+', L'-', L'+');
            break;
        case BoxType::Box:
            //Box(x, y, width, height, _curAttr, L'┌', L'─', L'┐', L'│', L'│', L'└', L'─', L'┘');
            Box(x, y, width, height, _curAttr, 0x250c, 0x2500, 0x2510, 0x2502, 0x2502, 0x2514, 0x2500, 0x2518);
            //Box(x, y, width, height, _curAttr, u'┌', u'─', u'┐', u'│', u'│', u'└', u'─', u'┘');
            break;
        }
    }

    void Console::Box(short x, short y, short width, short height, Attr attr, BoxType box_type) {
        switch(box_type) {
        case BoxType::Ascii:
            Box(x, y, width, height, attr, L'+', L'-', L'+', L'|', L'|', L'+', L'-', L'+');
            break;
        case BoxType::Box:
            //Box(x, y, width, height, attr, L'┌', L'─', L'┐', L'│', L'│', L'└', L'─', L'┘');
            Box(x, y, width, height, attr, 0x250c, 0x2500, 0x2510, 0x2502, 0x2502, 0x2514, 0x2500, 0x2518);
            break;
        }
    }

    void Console::Box(short x, short y, short width, short height, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) {
        Box(x, y, width, height, _curAttr, tl, t, tr, l, r, bl, b, br);
    }

    void Console::Box(short x, short y, short width, short height, Attr attr, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) {
        for(int i = x + 1; i < x + width - 1; ++i) {
            SetChar(i, y, t, attr);
            SetChar(i, y + height - 1, b, attr);
        }
        for(int i = y + 1; i < y + height - 1; ++i) {
            SetChar(x, i, l, attr);
            SetChar(x + width - 1, i, r, attr);
        }

        SetChar(x, y, tl, attr);
        SetChar(x + width - 1, y, tr, attr);
        SetChar(x, y + height - 1, bl, attr);
        SetChar(x + width - 1, y + height - 1, br, attr);
    }

    void Console::Fill(short x, short y, short width, short height, wchar_t ch, Attr attr) {
        Fill(x, y, width, height, CChar{ch, (unsigned short)attr});
    }

    void Console::Fill(short x, short y, short width, short height, CChar ch) {
        for(int j = 0; j < height; ++j) {
            for(int i = 0; i < width; ++i) {
                SetChar(x + i, y + j, ch);
            }
        }
    }

    void Console::Display() {
        //if(_dirty) {
        //    auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        //    auto buf_size = COORD{_width, _height};
        //    auto buf_coord = COORD{0, 0};
        //    auto write_region = SMALL_RECT{0, 0, _width - 1, _height - 1};
        //    WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
        //}
        //_dirty = false;

        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if(_dirty) {
            auto buf_size = COORD{_width, _height};
            auto buf_coord = COORD{0, 0};
            auto write_region = SMALL_RECT{0, 0, _width - 1, _height - 1};
            WriteConsoleOutputW(hOut, _backBuf.data(), buf_size, buf_coord, &write_region);
            _dirty = false;
            flipBuffer();
            return;
        }
#ifdef DISPLAY_STRATEGY_WHOLE
        for(int y = 0; y < _height; ++y) {
            for(int x = 0; x < _width; ++x) {
                if(_dirtyBuf[x + y * _width]) {
                    auto buf_size = COORD{_width, _height};
                    auto buf_coord = COORD{(short)x, (short)y};
                    auto write_region = SMALL_RECT{(short)x, (short)y, (short)(x), (short)(y)};
                    WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
                }
            }
        }
#elif defined(DISPLAY_STRATEGY_SMART)
        auto buf_size = COORD{_width, _height};
        auto rects = compileDirtyRects();
        if(totalAreaOfDirtyRects(rects) <= (_width * _height) / 4) {
            for(auto & rect : compileDirtyRects()) {
                auto buf_coord = COORD{rect.Left, rect.Top};
                auto write_region = rect;
                WriteConsoleOutputW(hOut, _backBuf.data(), buf_size, buf_coord, &write_region);
            }
        } else {
            auto buf_coord = COORD{0,0};
            auto write_region = SMALL_RECT{0, 0, _width - 1, _height - 1};
            WriteConsoleOutputW(hOut, _backBuf.data(), buf_size, buf_coord, &write_region);
        }
#elif defined(DISPLAY_STRATEGY_TUNED)
        const int SPLIT = 4;
        int REAL_SPLIT = std::min((int)std::min(_width, _height), SPLIT);
        auto buf_size = COORD{_width, _height};
        for(int y = 0; y < REAL_SPLIT; ++y) {
            for(int x = 0; x < REAL_SPLIT; ++x) {
                if(x != REAL_SPLIT - 1) {
                    if(y != REAL_SPLIT - 1) {
                        auto buf_coord = COORD{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT))};
                        auto write_region = SMALL_RECT{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT)),
                            (SHORT)(((x + 1) * (_width / REAL_SPLIT)) - 1),
                            (SHORT)(((y + 1) * (_height / REAL_SPLIT)) - 1)};
                        if(IsRectDirty(write_region)) {
                            WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
                        }
                    } else {
                        auto buf_coord = COORD{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT))};
                        auto write_region = SMALL_RECT{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT)),
                            (SHORT)(((x + 1) * (_width / REAL_SPLIT)) - 1),
                            _height - 1};
                        if(IsRectDirty(write_region)) {
                            WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
                        }
                    }
                } else {
                    if(y != REAL_SPLIT - 1) {
                        auto buf_coord = COORD{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT))};
                        auto write_region = SMALL_RECT{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT)),
                            (SHORT)(_width - 1),
                            (SHORT)(((y + 1) * (_height / REAL_SPLIT)) - 1)};
                        if(IsRectDirty(write_region)) {
                            WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
                        }
                    } else {
                        auto buf_coord = COORD{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT))};
                        auto write_region = SMALL_RECT{
                            (SHORT)(x * (_width / REAL_SPLIT)),
                            (SHORT)(y * (_height / REAL_SPLIT)),
                            (SHORT)(_width - 1),
                            (SHORT)(_height - 1)};
                        if(IsRectDirty(write_region)) {
                            WriteConsoleOutputW(hOut, _buffer.data(), buf_size, buf_coord, &write_region);
                        }
                    }
                }
            }
        }
#endif

        //ClearAllDirty();
        flipBuffer();
    }

    bool Console::CursorVisible() const {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hOut, &cci);
        return cci.bVisible != 0;
    }

    void Console::SetCursorVisible(bool visible) {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hOut, &cci);
        cci.bVisible = visible;
        SetConsoleCursorInfo(hOut, &cci);
    }

    short Console::CursorX() const {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        return csbi.dwCursorPosition.X;
    }

    short Console::CursorY() const {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        return csbi.dwCursorPosition.Y;
    }

    void Console::SetCursorX(short x) {
        SetCursorPosition(x, CursorY());
    }

    void Console::SetCursorY(short y) {
        SetCursorPosition(CursorX(), y);
    }

    void Console::CursorPosition(short & x, short & y) const {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        x = csbi.dwCursorPosition.X;
        y = csbi.dwCursorPosition.Y;
    }

    void Console::SetCursorPosition(short x, short y) {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        auto pos = COORD{x, y};
        SetConsoleCursorPosition(hOut, pos);
    }

    std::wstring Console::Title() const {
        std::wstring ret;
        static wchar_t tmpstr[128] = { };
        wmemset(tmpstr, L'\0', 128);
        GetConsoleTitleW(tmpstr, 127);
        ret = std::wstring(tmpstr);
        return ret;
    }

    void Console::SetTitle(std::wstring const & str) {
        SetConsoleTitleW(str.c_str());
    }

    bool Console::IsDirty() const {
        //return _dirty;
        //auto iter = std::find(std::begin(_dirtyBuf), std::end(_dirtyBuf), true);
        //if(iter != std::end(_dirtyBuf)) {
        //    return true;
        //}
        //return false;
        if(_dirty) { return true; }
        else {
            bool dirty = false;
            for(int i = 0; i < _width * _height; ++i) {
                if(!Equal(_buffer[i], _backBuf[i])) {
                    return true;
                }
            }
            return false;
        }
    }

    bool Console::IsRectDirty(SMALL_RECT const & rect) const {
        if(_dirty) { return true; }
        for(int y = rect.Top; y <= rect.Bottom; ++y) {
            for(int x = rect.Left; x <= rect.Right; ++x) {
                if(!Equal(_buffer[x + y * _width], _backBuf[x + y * _width])) {
                    return true;
                }
            }
        }
        return false;
    }

    //void Console::SetIsDirty(bool is_dirty) {
    //    //_dirty = is_dirty;
    //}

    void Console::SetAllDirty() {
        //_dirtyBuf.assign(_dirtyBuf.size(), true);
        _dirty = true;
    }

    void Console::ClearAllDirty() {
        //_dirtyBuf.assign(_dirtyBuf.size(), false);
        _dirty = false;
    }

    void Console::GetPalette(COLORREF color_table[16]) const {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX csbix;
        csbix.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        GetConsoleScreenBufferInfoEx(hOut, &csbix);
        memcpy(color_table, csbix.ColorTable, 16 * sizeof(COLORREF));
    }

    void Console::SetPalette(COLORREF const color_table[16]) {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFOEX csbix;
        csbix.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        GetConsoleScreenBufferInfoEx(hOut, &csbix);
        memcpy(csbix.ColorTable, color_table, 16 * sizeof(COLORREF));
        csbix.srWindow.Right++;
        csbix.srWindow.Bottom++;
        SetConsoleScreenBufferInfoEx(hOut, &csbix);
    }

    std::vector<SMALL_RECT> Console::compileDirtyRects() {
        std::vector<SMALL_RECT> vec = { };

        // Make row-based list until a better algorithm can be found
        for(int y = 0; y < _height; ++y) {
            int x = 0;
            int next_x = 0;
            bool rect_found = false;
            for(x = 0; x < _width; x = next_x) {
                SMALL_RECT rect = { };
                //if(_dirtyBuf[x + y * _width]) {
                if(!Equal(_buffer[x + y * _width], _backBuf[x + y * _width])) {
                    rect_found = true;
                    rect.Left = x;
                    rect.Top = y;
                    rect.Bottom = y;
                    for(int x2 = x; x2 < _width; ++x2) {
                        next_x = x2 + 1;
                        //if(!_dirtyBuf[x2 + y * _width]) {
                        if(Equal(_buffer[x + y * _width], _backBuf[x + y * _width])) {
                            break;
                        } else {
                            rect.Right = x2;
                        }
                    }
                    vec.push_back(rect);
                } else {
                    next_x = x + 1;
                }
            }
        }

        //static wchar_t buf[64] = { };
        //wmemset(buf, L'\0', 64);
        //_ui64tow(vec.size(), buf, 10);
        //SetTitle(std::wstring(buf));

        return vec;
    }

    unsigned long long Console::totalAreaOfDirtyRects(std::vector<SMALL_RECT> const & rects) const {
        unsigned long long ret = 0;
        for(auto & rect : rects) {
            auto width = rect.Right - rect.Left + 1;
            auto height = rect.Bottom - rect.Top + 1;
            ret += width * height;
        }
        return ret;
    }

    void Console::flipBuffer() {
        memcpy(_buffer.data(), _backBuf.data(), _backBuf.size() * sizeof(CChar));
    }

    void Console::resizeConsole(short width, short height) {
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        bool success;
        SMALL_RECT window_rect;
        COORD max_window;
        success = GetConsoleScreenBufferInfo(hOut, &csbi) == TRUE;
        max_window = GetLargestConsoleWindowSize(hOut);
        window_rect.Right = std::min(width, max_window.X) - 1;
        window_rect.Bottom = std::min(height, max_window.Y) - 1;
        window_rect.Left = 0;
        window_rect.Top = 0;
        max_window.X = width;
        max_window.Y = height;
        auto window_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        auto window_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if(width > window_width) {
            if(height > window_height) {
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
            } else if(height == window_height) {
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
            } else if(height < window_height) {
                auto tmp = max_window;
                tmp.Y = tmp.X;
                success = SetConsoleScreenBufferSize(hOut, tmp) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
            }
        } else if(width == window_width) {
            if(height > window_height) {
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
            } else if(height == window_height) {
                // Shouldn't be needed except that this could help
                // the console get out of a weird state
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
            } else if(height < window_height) {
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
            }
        } else if(width < window_width) {
            if(height > window_height) {
                auto tmp = max_window;
                tmp.X = tmp.Y;
                success = SetConsoleScreenBufferSize(hOut, tmp) == TRUE;
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
            } else if(height == window_height) {
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
            } else if(height < window_height) {
                success = SetConsoleWindowInfo(hOut, true, &window_rect) == TRUE;
                success = SetConsoleScreenBufferSize(hOut, max_window) == TRUE;
            }
        }
    }

    void Console::resizeBuf(short width, short height) {
        _dirty = true;
        _buffer.resize(width * height);
        _backBuf.resize(width * height);
        //_dirtyBuf.resize(width * height);
        Clear();
    }

    unsigned int Console::NumberOfMouseButtons() const {
        DWORD ret;
        GetNumberOfConsoleMouseButtons(&ret);
        return ret;
    }

    bool Console::GetEvent(INPUT_RECORD & newEvent) {
        auto hIn = GetStdHandle(STD_INPUT_HANDLE);
        if(NumberOfEvents() >= 1) {
            INPUT_RECORD tmp;
            DWORD num;
            ReadConsoleInput(hIn, &tmp, 1, &num);
            newEvent = tmp;
            return true;
        }
        return false;
    }

    int Console::NumberOfEvents() const {
        auto hIn = GetStdHandle(STD_INPUT_HANDLE);
        DWORD num;
        GetNumberOfConsoleInputEvents(hIn, &num);
        return num;
    }

    INPUT_RECORD Console::WaitForEvent() {
        INPUT_RECORD evt;
        for(;;) {
            while(GetEvent(evt)) {
                return evt;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(1000.0 / 15.0)));
        }
    }

    void Console::WaitForKey(unsigned int virtualKeyCode) {
        INPUT_RECORD evt;
        for(;;) {
            while(GetEvent(evt)) {
                switch(evt.EventType) {
                case KEY_EVENT:
                    if(evt.Event.KeyEvent.bKeyDown) {
                        if(evt.Event.KeyEvent.wVirtualKeyCode == virtualKeyCode) {
                            return;
                        }
                    }
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(1000.0 / 15.0)));
        }
    }

    void Console::WaitForEnter() {
        WaitForKey(VK_RETURN);
    }

}
