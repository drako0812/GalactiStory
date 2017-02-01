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

#include "GalactiQuestBase.hpp"
#include "Vector2.hpp"

#include <algorithm>

namespace gquest {

    template <class Type>
    class Rect {
    public:
        Type Left;
        Type Top;
        Type Width;
        Type Height;

        constexpr Rect() : Left(), Top(), Width(), Height() { }
        constexpr Rect(Type const& left, Type const& top, Type const& width, Type const& height) :
            Left(left), Top(Top), Width(width), Height(height) { }
        constexpr Rect(Vector2<Type> const& left_top, Vector2<Type> const& size) :
            Left(left_top.X), Top(left_top.Y), Width(size.X), Height(size.Y) { }
        constexpr Rect(Rect const& rect) : Left(rect.Left), Top(rect.Top), Width(rect.Width), Height(rect.Height) { }
        constexpr Rect(Type && left, Type && top, Type && width, Type && height) :
            Left(left), Top(top), Width(width), Height(height) { }
        constexpr Rect(Vector2<Type> && left_top, Vector2<Type> && size) :
            Left(left_top.X), Top(left_top.Y), Width(size.X), Height(size.Y) { }
        constexpr Rect(Rect && rect) : Left(rect.Left), Top(rect.Top), Width(rect.Width), Height(rect.Height) { }

        // Implementation based on SFML https://sfml-dev.org
        inline constexpr bool contains(Type const& x, Type const& y) const {
            return
                (x >= std::min(Left, Left + Width)) &&
                (x < std::max(Left, Left + Width)) &&
                (y >= std::min(Top, Top + Height)) &&
                (y < std::max(Top, Top + Height));
        }

        // Implementation based on SFML https://sfml-dev.org
        inline constexpr bool contains(Vector2<Type> const& point) const {
            return contains(point.X, point.Y);
        }

        // Implementation based on SFML https://sfml-dev.org
        inline constexpr bool intersects(Rect const& rect) const {
            constexpr Type r1MinX = std::min(Left, Left + Width);
            constexpr Type r1MaxX = std::max(Left, Left + Width);
            constexpr Type r1MinY = std::min(Top, Top + Height);
            constexpr Type r1MaxY = std::max(Top, Top + Height);

            constexpr Type r2MinX = std::min(rect.Left, rect.Left + rect.Width);
            constexpr Type r2MaxX = std::max(rect.Left, rect.Left + rect.Height);
            constexpr Type r2MinY = std::min(rect.Top, rect.Top + rect.Height);
            constexpr Type r2MaxY = std::max(rect.Top, rect.Top + rect.Height);

            constexpr Type interLeft = std::max(r1MinX, r2MinX);
            constexpr Type interTop = std::max(r1MinY, r2MinY);
            constexpr Type interRight = std::min(r1MaxX, r2MaxX);
            constexpr Type interBottom = std::min(r1MaxY, r2MaxY);

            if((interLeft < interRight) && (interTop < interBottom)) {
                return true;
            }
            return false;
        }
    };

    using URect = Rect<uint_>;
    using IRect = Rect<int_>;
    using FRect = Rect<float_>;

}
