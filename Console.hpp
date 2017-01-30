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

#include "ConLibBase.hpp"
#include "IConsole.hpp"

namespace conlib {

    class Console : public IConsole {
    private:
        std::vector<CChar> _buffer;
        std::vector<CChar> _backBuf;

        Console();

    public:
        virtual ~Console() override;

    private:
        static std::unique_ptr<Console> _instance;

    public:
        static Console * Get();

    private:
        short _oldBufWidth;
        short _oldBufHeight;
        Attr _oldAttr;
        short _oldWinWidth;
        short _oldWinHeight;
        Attr _curAttr;
        short _width;
        short _height;
        DWORD _oldOutMode;
        DWORD _oldInMode;
        bool _dirty;
        bool _init;
        UINT _oldInputCodePage;
        UINT _oldOutputCodePage;
        COLORREF _oldPalette[16];
        //std::vector<bool> _dirtyBuf;

    public:
        void Initialize();
        CChar GetChar(short x, short y) const override;
        short Width() const override;
        short Height() const override;
        void SetWidth(short width) override;
        void SetHeight(short height) override;
        void Size(short & width, short & height) const override;
        void Resize(short width, short height) override;
        Attr CurrentAttr() const override;
        void SetCurrentAttr(Attr attr) override;
        void SetChar(short x, short y, CChar ch) override;
        void SetChar(short x, short y, wchar_t ch) override;
        void SetChar(short x, short y, wchar_t ch, Attr attr) override;
        void PutString(short x, short y, std::wstring const& str, Attr attr, int max_length = -1) override;
        void PutString(short x, short y, std::wstring const& str, int max_length = -1) override;
        template <class RandomAccessContainerCChar> void PutString(short x, short y, RandomAccessContainerCChar const& str, int max_length = -1);
        //template <class IterableCChar> void PutString(short x, short y, , int max_length, IterableCChar const& str);
        void Clear() override;
        void Clear(wchar_t fill_ch) override;
        void Clear(CChar fill_ch) override;
        void Clear(wchar_t fill_ch, Attr attr) override;
        void Box(short x, short y, short width, short height, BoxType box_type = BoxType::Box) override;
        void Box(short x, short y, short width, short height, Attr attr, BoxType box_type = BoxType::Box) override;
        void Box(short x, short y, short width, short height, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) override;
        void Box(short x, short y, short width, short height, Attr attr, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) override;
        void Fill(short x, short y, short width, short height, wchar_t ch, Attr attr) override;
        void Fill(short x, short y, short width, short height, CChar ch) override;
        void Display();
        bool CursorVisible() const;
        void SetCursorVisible(bool visible);
        short CursorX() const;
        short CursorY() const;
        void SetCursorX(short x);
        void SetCursorY(short y);
        void CursorPosition(short & x, short & y) const;
        void SetCursorPosition(short x, short y);
        std::wstring Title() const;
        void SetTitle(std::wstring const& str);
        bool IsDirty() const;
        bool IsRectDirty(SMALL_RECT const& rect) const;
        //void SetIsDirty(bool is_dirty);
        void SetAllDirty();
        void ClearAllDirty();

        void GetPalette(COLORREF color_table[16]) const;
        void SetPalette(COLORREF const color_table[16]);

    private:

        std::vector<SMALL_RECT> compileDirtyRects();
        unsigned long long totalAreaOfDirtyRects(std::vector<SMALL_RECT> const& rects) const;
        void flipBuffer();
        //void lookRight(std::vector<SMALL_RECT> & vec, int startx, int starty, int maxx, int maxy);
        //void lookDown(std::vector<SMALL_RECT> & vec, int startx, int starty, int maxx, int maxy);
        void resizeConsole(short width, short height);
        void resizeBuf(short width, short height);

    public:

        unsigned int NumberOfMouseButtons() const;
        template<class Iterable_INPUT_RECORD> void GenerateEvents(Iterable_INPUT_RECORD events);
        bool GetEvent(INPUT_RECORD & newEvent);
        int NumberOfEvents() const;
        template<class PushBackableContainer_INPUT_RECORD> void GetEvents(PushBackableContainer_INPUT_RECORD & events, unsigned int max_events);
        INPUT_RECORD WaitForEvent();
        void WaitForKey(unsigned int virtualKeyCode);
        void WaitForEnter();
    };

    template <class RandomAccessContainerCChar> void Console::PutString(short x,
        short y, RandomAccessContainerCChar const& str, int max_length) {

        if(max_length < 0) { max_length = (int)std::min((std::size_t)INT32_MAX, str.size()); }
        int start_idx = x + y * _width;
        CChar tmpch;
        for(int i = 0; (i < str.size()) && (i < max_length) && (i + start_idx < _buffer.size()); ++i) {
            tmpch = str[i];
            //if(_buffer[i + start_idx] != tmpch) {
            if(!Equal(_buffer[i + start_idx], tmpch)) {
                _buffer[i + start_idx] = tmpch;
                //_dirty = true;
                _dirtyBuf[i + start_idx] = true;
            }
        }
    }

    /*template <class IterableCChar> void Console::PutString(short x, short y,
    int max_length, IterableCChar const& str) {

    if(max_length < 0) { max_length = INT32_MAX; }
    int idx = 0;
    int start_idx = x + y * _width;
    CChar tmpch;
    for(auto iter = std::begin(str);
    (iter != std::end(str)) &&
    (idx < max_length) &&
    (idx + start_idx < _buffer.size());
    ++iter, ++idx) {

    tmpch = *iter;
    if(_buffer[i + start_idx] != tmpch) {
    _buffer[i + start_idx] = tmpch;
    _dirty = true;
    }
    }
    }*/

    template <class PushBackableContainer_INPUT_RECORD> void Console::GetEvents(
        PushBackableContainer_INPUT_RECORD & events, unsigned int max_events) {

        auto hIn = GetStdHandle(STD_INPUT_HANDLE);
        std::vector<INPUT_RECORD> tmp;
        tmp.resize(max_events);
        unsigned int count;
        ReadConsoleInputW(hIn, tmp.data(), max_events, &count);
        for(auto & evt : tmp) {
            events.push_back(evt);
        }
    }

    template<class Iterable_INPUT_RECORD> void Console::GenerateEvents(Iterable_INPUT_RECORD events) {
        auto hIn = GetStdHandle(STD_INPUT_HANDLE);
        DWORD num;
        std::vector<INPUT_RECORD> vec;
        for(auto & evt : events) {
            vec.push_back(evt);
        }
        WriteConsoleInputW(hIn, vec.data(), vec.size(), &num);
    }

}
