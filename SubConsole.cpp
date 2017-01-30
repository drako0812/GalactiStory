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
#include "SubConsole.hpp"

namespace conlib {

    SubConsole::SubConsole(short width, short height, Attr attr) :
        _curAttr(attr), _width(width), _height(height) {

        _buffer.resize(width * height);
        _buffer.assign(width * height, CChar{L' ', attr});
    }

    SubConsole::~SubConsole() {
        _buffer.clear();
    }

    CChar SubConsole::GetChar(short x, short y) const {
        return _buffer[x + y * _width];
    }

    short SubConsole::Width() const {
        return _width;
    }

    short SubConsole::Height() const {
        return _height;
    }

    void SubConsole::SetWidth(short width) {
        Resize(width, _height);
    }

    void SubConsole::SetHeight(short height) {
        Resize(_width, height);
    }

    void SubConsole::Size(short & width, short & height) const {
        width = _width;
        height = _height;
    }

    void SubConsole::Resize(short width, short height) {
        _width = width;
        _height = height;
        _buffer.resize(width * height);
        _buffer.assign(width * height, CChar{L' ', _curAttr});
    }

    Attr SubConsole::CurrentAttr() const {
        return _curAttr;
    }

    void SubConsole::SetCurrentAttr(Attr attr) {
        _curAttr = attr;
    }

    void SubConsole::SetChar(short x, short y, CChar ch) {
        if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) {
            return;
        }
        if(!Equal(_buffer[x + y * _width], ch)) {
            _buffer[x + y * _width] = ch;
        }
    }

    void SubConsole::SetChar(short x, short y, wchar_t ch) {
        SetChar(x, y, CChar{ch, _curAttr});
    }

    void SubConsole::SetChar(short x, short y, wchar_t ch, Attr attr) {
        SetChar(x, y, CChar{ch, attr});
    }

    void SubConsole::PutString(short x, short y, std::wstring const & str, Attr attr, int max_length) {
        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.length()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.length()) && (i < max_length) && (i + start_idx < _buffer.size()); ++i) {
            tmpch.Char.UnicodeChar = str[i];
            tmpch.Attributes = attr;
            if(!Equal(_buffer[i + start_idx], tmpch)) {
                _buffer[i + start_idx] = tmpch;
            }
        }
    }

    void SubConsole::PutString(short x, short y, std::wstring const & str, int max_length) {
        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.length()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.length()) && (i < max_length) && (i + start_idx < _buffer.size()); ++i) {
            tmpch.Char.UnicodeChar = str[i];
            tmpch.Attributes = _curAttr;
            if(!Equal(_buffer[i + start_idx], tmpch)) {
                _buffer[i + start_idx] = tmpch;
            }
        }
    }

    void SubConsole::Clear() {
        CChar ch;
        ch.Char.UnicodeChar = L' ';
        ch.Attributes = _curAttr;
        Clear(ch);
    }

    void SubConsole::Clear(wchar_t fill_ch) {
        CChar ch;
        ch.Char.UnicodeChar = fill_ch;
        ch.Attributes = _curAttr;
        Clear(ch);
    }

    void SubConsole::Clear(CChar fill_ch) {
        for(int y = 0; y < _height; ++y) {
            for(int x = 0; x < _width; ++x) {
                SetChar(x, y, fill_ch);
            }
        }
    }

    void SubConsole::Clear(wchar_t fill_ch, Attr attr) {
        CChar ch;
        ch.Char.UnicodeChar = fill_ch;
        ch.Attributes = attr;
        Clear(ch);
    }

    void SubConsole::Box(short x, short y, short width, short height, BoxType box_type) {
        switch(box_type) {
        case BoxType::Ascii:
            Box(x, y, width, height, _curAttr, L'+', L'-', L'+', L'|', L'|', L'+', L'-', L'+');
            break;
        case BoxType::Box:
            Box(x, y, width, height, _curAttr, 0x250c, 0x2500, 0x2510, 0x2502, 0x2502, 0x2514, 0x2500, 0x2518);
            break;
        }
    }

    void SubConsole::Box(short x, short y, short width, short height, Attr attr, BoxType box_type) {
        switch(box_type) {
        case BoxType::Ascii:
            Box(x, y, width, height, attr, L'+', L'-', L'+', L'|', L'|', L'+', L'-', L'+');
            break;
        case BoxType::Box:
            Box(x, y, width, height, attr, 0x250c, 0x2500, 0x2510, 0x2502, 0x2502, 0x2514, 0x2500, 0x2518);
            break;
        }
    }

    void SubConsole::Box(short x, short y, short width, short height, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) {
        Box(x, y, width, height, _curAttr, tl, t, tr, l, r, bl, b, br);
    }

    void SubConsole::Box(short x, short y, short width, short height, Attr attr, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) {
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

    void SubConsole::Fill(short x, short y, short width, short height, wchar_t ch, Attr attr) {
        Fill(x, y, width, height, CChar{ch, (unsigned short)attr});
    }

    void SubConsole::Fill(short x, short y, short width, short height, CChar ch) {
        for(int j = 0; j < height; ++j) {
            for(int i = 0; i < width; ++i) {
                SetChar(x + i, y + j, ch);
            }
        }
    }

}
