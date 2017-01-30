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

namespace conlib {

    class IConsole {
    public:
        virtual ~IConsole();
        virtual CChar GetChar(short x, short y) const = 0;
        virtual short Width() const = 0;
        virtual short Height() const = 0;
        virtual void SetWidth(short width) = 0;
        virtual void SetHeight(short height) = 0;
        virtual void Size(short & width, short & height) const = 0;
        virtual void Resize(short width, short height) = 0;
        virtual Attr CurrentAttr() const = 0;
        virtual void SetCurrentAttr(Attr attr) = 0;
        virtual void SetChar(short x, short y, CChar ch) = 0;
        virtual void SetChar(short x, short y, wchar_t ch) = 0;
        virtual void SetChar(short x, short y, wchar_t ch, Attr attr) = 0;
        virtual void PutString(short x, short y, std::wstring const& str, Attr attr, int max_length = -1) = 0;
        virtual void PutString(short x, short y, std::wstring const& str, int max_length = -1) = 0;
        //template <class RandomAccessContainerCChar> void PutString(short x, short y, RandomAccessContainerCChar const& str, int max_length = -1) = 0;
        virtual void Clear() = 0;
        virtual void Clear(wchar_t fill_ch) = 0;
        virtual void Clear(CChar fill_ch) = 0;
        virtual void Clear(wchar_t fill_ch, Attr attr) = 0;
        virtual void Box(short x, short y, short width, short height, BoxType box_type = BoxType::Box) = 0;
        virtual void Box(short x, short y, short width, short height, Attr attr, BoxType box_type = BoxType::Box) = 0;
        virtual void Box(short x, short y, short width, short height, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) = 0;
        virtual void Box(short x, short y, short width, short height, Attr attr, wchar_t tl, wchar_t t, wchar_t tr, wchar_t l, wchar_t r, wchar_t bl, wchar_t b, wchar_t br) = 0;
        virtual void Fill(short x, short y, short width, short height, wchar_t ch, Attr attr) = 0;
        virtual void Fill(short x, short y, short width, short height, CChar ch) = 0;

        static void Blit(IConsole & dest, IConsole & src, SMALL_RECT const& dest_rect, SMALL_RECT const& src_rect, wchar_t ignore_character = L'\0');
    };

}
