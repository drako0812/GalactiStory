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
#include "IConsole.hpp"

namespace conlib {

    IConsole::~IConsole() { }

    void IConsole::Blit(IConsole & dest, IConsole & src, SMALL_RECT const & dest_rect, SMALL_RECT const & src_rect, wchar_t ignore_character) {
        auto dest_w = dest_rect.Right - dest_rect.Left;
        auto dest_h = dest_rect.Bottom - dest_rect.Top;
        auto src_w = src_rect.Right - src_rect.Left;
        auto src_h = src_rect.Bottom - src_rect.Top;
        auto width = std::min(dest_w, src_w);
        auto height = std::min(dest_h, src_h);
        for(int j = 0; j < height; ++j) {
            for(int i = 0; i < width; ++i) {
                CChar src_ch = src.GetChar(src_rect.Left + i, src_rect.Top + j);
                if(ignore_character != L'\0') {
                    if(ignore_character == src_ch.Char.UnicodeChar) {
                        continue;
                    }
                }
                dest.SetChar(dest_rect.Left + i, dest_rect.Top + j, src_ch);
            }
        }
    }

}
