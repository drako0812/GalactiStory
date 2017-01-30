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

namespace gquest {

    using namespace conlib;

    constexpr COLORREF default_palette[16] = {
        0x00000000,
        0x00800000,
        0x00008000,
        0x00808000,
        0x00000080,
        0x00800080,
        0x00008080,
        0x00808080,
        0x00404040,
        0x00FF0000,
        0x0000FF00,
        0x00FFFF00,
        0x000000FF,
        0x00FF00FF,
        0x0000FFFF,
        0x00FFFFFF,
    };

    constexpr COLORREF Softened_Pal[16] = {
        Rgb(0, 0, 0),
        Rgb(0, 0, 96),
        Rgb(0, 96, 0),
        Rgb(0, 96, 96),
        Rgb(96, 0, 0),
        Rgb(96, 0, 96),
        Rgb(96, 96, 0),
        Rgb(128, 128, 128),
        Rgb(96, 96, 96),
        Rgb(32, 32, 255),
        Rgb(32, 255, 32),
        Rgb(64, 255, 255),
        Rgb(255, 32, 32),
        Rgb(255, 64, 255),
        Rgb(255, 255, 64),
        Rgb(255, 255, 255)
    };

}
