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

#include <sstream>
#include <string>

#include <Windows.h>

namespace conlib {

    constexpr COLORREF Rgb(unsigned char r, unsigned char g, unsigned char b) {
        return
            (0x00000000) |
            ((int)r) |
            ((int)g << 8) |
            ((int)b << 16);
    }

    enum Attr : unsigned short {
        None = 0,

        BgRed = BACKGROUND_RED,
        BgYellow = BACKGROUND_RED | BACKGROUND_GREEN,
        BgGreen = BACKGROUND_GREEN,
        BgCyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
        BgBlue = BACKGROUND_BLUE,
        BgMagenta = BACKGROUND_BLUE | BACKGROUND_RED,
        BgLightRed = BACKGROUND_RED | BACKGROUND_INTENSITY,
        BgLightYellow = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
        BgLightGreen = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
        BgLightCyan = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BgLightBlue = BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BgLightMagenta = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY,
        BgBlack = 0,
        BgGrey = BACKGROUND_INTENSITY,
        BgLightGrey = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        BgWhite = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,

        FgRed = FOREGROUND_RED,
        FgYellow = FOREGROUND_RED | FOREGROUND_GREEN,
        FgGreen = FOREGROUND_GREEN,
        FgCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
        FgBlue = FOREGROUND_BLUE,
        FgMagenta = FOREGROUND_BLUE | FOREGROUND_RED,
        FgLightRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
        FgLightYellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FgLightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FgLightCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FgLightBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FgLightMagenta = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
        FgBlack = 0,
        FgGrey = FOREGROUND_INTENSITY,
        FgLightGrey = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        FgWhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,

        Pal0 = 0,
        Pal1,
        Pal2,
        Pal3,
        Pal4,
        Pal5,
        Pal6,
        Pal7,
        Pal8,
        Pal9,
        Pal10,
        Pal11,
        Pal12,
        Pal13,
        Pal14,
        Pal15,
    };

    inline Attr operator |(Attr attr1, Attr attr2) {
        return (Attr)((unsigned short)attr1 | (unsigned short)attr2);
    }

    inline Attr operator &(Attr attr1, Attr attr2) {
        return (Attr)(attr1 & attr2);
    }

    inline Attr operator ^(Attr attr1, Attr attr2) {
        return (Attr)(attr1 ^ attr2);
    }

    inline Attr operator ~(Attr attr) {
        return (Attr)(~attr);
    }

    inline Attr operator <<(Attr attr, int shift) {
        return (Attr)(attr << shift);
    }

    inline Attr operator >> (Attr attr, int shift) {
        return (Attr)(attr >> shift);
    }

    enum class Blend {
        Step0,
        Step1,
        Step2,
        Step3,
        Step4,
    };

    enum class BoxType {
        Ascii,
        Box,
    };

    using CChar = CHAR_INFO;

    inline bool Equal(CChar left, CChar right) {
        return
            (left.Attributes == right.Attributes) &&
            (left.Char.UnicodeChar == right.Char.UnicodeChar);
    }

    inline CChar GetBlendedColor(int attr, Blend blend) {
        return GetBlendedColor((Attr)attr, blend);
    }

    inline CChar GetBlendedColor(Attr attr, Blend blend) {
        switch(blend) {
        case Blend::Step0:
            return CChar{L' ', attr};
            break;
        case Blend::Step1:
            return CChar{0x2591, attr};
            break;
        case Blend::Step2:
            return CChar{0x2592, attr};
            break;
        case Blend::Step3:
            return CChar{0x2593, attr};
            break;
        case Blend::Step4:
            return CChar{0x2588, attr};
            break;
        default:
            return CChar{L'?', attr};
        }
    }

    template <class ToType> inline ToType FromString(std::wstring const& str) {
        static std::wistringstream wiss;
        wiss.str(str);
        ToType ret;
        wiss >> ret;
        return ret;
    }

    template <class FromType> inline std::wstring ToString(FromType const& value) {
        static std::wostringstream woss;
        woss.str(L"");
        woss << value;
        return woss.str();
    }

}
