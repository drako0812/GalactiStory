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

#include <cmath>

namespace gquest {

    template <class Type>
    class Vector2 {
    public:
        Type X;
        Type Y;

        constexpr Vector2() : X(), Y() { }
        constexpr Vector2(Type const& X, Type const& Y) : X(X), Y(Y) { }
        constexpr Vector2(Vector2 const& vec) : X(vec.X), Y(vec.Y) { }
        constexpr Vector2(Type && X, Type && Y) : X(std::move(X)), Y(std::move(Y)) { }
        constexpr Vector2(Vector2 && vec) : X(std::move(vec.X)), Y(std::move(vec.Y)) { }

        inline constexpr Vector2 operator *(float_ scalar) const { return Vector2(static_cast<Type>(X*scalar), static_cast<Type>(Y*scalar)); }
        inline constexpr Vector2 operator +(Vector2 const& vec) const { return Vector2(X + vec.X, Y + vec.Y); }
        inline constexpr Vector2 operator -(Vector2 const& vec) const { return *this + (vec * -1.0); }
        inline constexpr float_ len() const { return std::sqrt(X * X + Y * Y); }
        inline constexpr float_ len2() const { return X * X + Y * Y; }
        inline constexpr Vector2 norm() const { return *this * (1.0 / len()); }
        inline constexpr float_ dot(Vector2 const& vec) const { return X * vec.X + Y * vec.Y; }
        inline constexpr bool collin(Vector2 const& vec) const {
            return approx_equal(this->dot(vec), (this->len() * vec.len()));
        }
        inline constexpr bool collino(Vector2 const& vec) const {
            return approx_equal(this->dot(vec), -1 * (this->len() * vec.len()));
        }
        inline constexpr bool perp(Vector2 const& vec) const {
            return approx_equal(this->dot(vec), 0);
        }
        inline constexpr bool samedir(Vector2 const& vec) const {
            return this->dot(vec) > 0;
        }
        inline constexpr bool oppdir(Vector2 const& vec) const {
            return this->dot(vec) < 0;
        }

        inline constexpr Vector2 lerp(Vector2 const& b, float_ beta) const {
            return Vector2(
                lerp(X, b.X, beta),
                lerp(Y, b.Y, beta)
            );
        }

        inline Vector2 & operator =(Vector2 const& vec) {
            X = vec.X;
            Y = vec.Y;
            return *this;
        }

    private:
        inline constexpr bool approx_equal(float_ a, float_ b) const {
            return (b < a + EQUALITY_TOLERANCE) && (b > a - EQUALITY_TOLERANCE) ? true : false;
        }
    };

    using UVector2 = Vector2<uint_>;
    using IVector2 = Vector2<int_>;
    using FVector2 = Vector2<float_>;

}
