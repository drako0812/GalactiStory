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

namespace gquest {

    template <class Type>
    class Vector3 {
    public:
        Type X;
        Type Y;
        Type Z;

        constexpr Vector3() : X(), Y(), Z() { }
        constexpr Vector3(Type const& X, Type const& Y, Type const& Z) : X(X), Y(Y), Z(Z) { }
        constexpr Vector3(Vector3 const& vec) : X(vec.X), Y(vec.Y), Z(vec.Z) { }
        constexpr Vector3(Type && X, Type && Y, Type && Z) : X(std::move(X)), Y(std::move(Y)), Z(std::move(Z)) { }
        constexpr Vector3(Vector3 && vec) : X(std::move(vec.X)), Y(std::move(vec.Y)), Z(std::move(vec.Z)) { }

        inline constexpr Vector3 operator *(float_ scalar) const {
            return Vector3(
                static_cast<Type>(X*scalar),
                static_cast<Type>(Y*scalar),
                static_cast<Type>(Z*scalar)
            );
        }

        inline constexpr Vector3 operator +(Vector3 const& vec) const {
            return Vector3(
                X + vec.X,
                Y + vec.Y,
                Z + vec.Z
            );
        }

        inline constexpr Vector3 operator -(Vector3 const& vec) const {
            return *this + (vec * -1.0);
        }

        inline constexpr float_ len() const { return std::sqrt(X * X + Y * Y + Z * Z); }
        inline constexpr float_ len2() const { return X * X + Y * Y + Z * Z; }
        inline constexpr Vector3 norm() const { return *this * (1.0 / len()); }
        inline constexpr float_ dot(Vector3 const& vec) const {
            return X * vec.X + Y * vec.Y + Z * vec.Z;
        }
        inline constexpr bool collin(Vector3 const& vec) const {
            return approx_equal(this->dot(vec), (this->len() * vec.len()));
        }
        inline constexpr bool collino(Vector3 const& vec) const {
            return approx_equal(this->dot(vec), -1 * (this->len() * vec.len()));
        }
        inline constexpr bool perp(Vector3 const& vec) const {
            return approx_equal(this->dot(vec), 0);
        }
        inline constexpr bool samedir(Vector3 const& vec) const {
            return this->dot(vec) > 0;
        }
        inline constexpr bool oppdir(Vector3 const& vec) const {
            return this->dot(vec) < 0;
        }

        inline constexpr Vector3 cross(Vector3 const& vec) const {
            return Vector3(
                Y * vec.Z - Z * vec.Y,
                Z * vec.X - X * vec.Z,
                X * vec.Y - Y * vec.X
            );
        }

        inline constexpr float_ area_rect(Vector3 const& vec) const {
            return this->cross(vec).len();
        }

        inline constexpr float_ area_tri(Vector3 const& vec) const {
            return 0.5 * this->cross(vec).len();
        }

        inline constexpr Vector3 lerp(Vector3 const& b, float_ beta) const {
            return Vector3(
                lerp(X, b.X, beta),
                lerp(Y, b.Y, beta),
                lerp(Z, b.Z, beta)
            );
        }

        inline Vector3 & operator =(Vector3 const& vec) {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
            return *this;
        }

    private:
        inline constexpr bool approx_equal(float_ a, float_ b) const {
            return (b < a + EQUALITY_TOLERANCE) && (b > a - EQUALITY_TOLERANCE) ? true : false;
        }
    };

    using UVector3 = Vector3<uint_>;
    using IVector3 = Vector3<int_>;
    using FVector3 = Vector3<float_>;

}
