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
        Rgb(0, 0, 84),
        Rgb(0, 84, 0),
        Rgb(0, 84, 84),
        Rgb(84, 0, 0),
        Rgb(84, 0, 84),
        Rgb(84, 84, 0),
        Rgb(171, 171, 171),
        Rgb(84, 84, 84),
        Rgb(84, 84, 255),
        Rgb(84, 255, 84),
        Rgb(84, 255, 255),
        Rgb(255, 84, 84),
        Rgb(255, 84, 255),
        Rgb(255, 255, 84),
        Rgb(255, 255, 255)
    };

    // Integer Types

    using ui8 = std::uint8_t;
    using ui16 = std::uint16_t;
    using ui32 = std::uint32_t;
    using ui64 = std::uint64_t;
    using uint_ = ui64;
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using int_ = i64;

    constexpr ui64 ui8_size = sizeof(ui8);
    constexpr ui64 ui16_size = sizeof(ui16);
    constexpr ui64 ui32_size = sizeof(ui32);
    constexpr ui64 ui64_size = sizeof(ui64);
    constexpr ui64 uint_size = sizeof(uint_);
    constexpr ui64 i8_size = sizeof(i8);
    constexpr ui64 i16_size = sizeof(i16);
    constexpr ui64 i32_size = sizeof(i32);
    constexpr ui64 i64_size = sizeof(i64);
    constexpr ui64 int_size = sizeof(int_);

    // Floating-point Types

    using f32 = float;
    using f64 = double;
    using fbig = long double;
    using float_ = fbig;

    constexpr ui64 f32_size = sizeof(f32);
    constexpr ui64 f64_size = sizeof(f64);
    constexpr ui64 fbig_size = sizeof(fbig);
    constexpr ui64 float_size = sizeof(float_);

    constexpr float_ EQUALITY_TOLERANCE = 0.0001;

    // String Type

    using string = std::wstring;

    // Dynamic array Types

    template <class Type> using vec = std::vector<Type>;
    using ui8vec = vec<ui8>;
    using ui16vec = vec<ui16>;
    using ui32vec = vec<ui32>;
    using ui64vec = vec<ui64>;
    using uvec = vec<uint_>;
    using i8vec = vec<ui8>;
    using i16vec = vec<ui16>;
    using i32vec = vec<ui32>;
    using i64vec = vec<ui64>;
    using ivec = vec<int_>;
    using f32vec = vec<f32>;
    using f64vec = vec<f64>;
    using fbigvec = vec<fbig>;
    using fvec = vec<float_>;
    using svec = vec<string>;
    
    // Fixed array Types

    template <class Type, ui64 Size> using array = std::array<Type, Size>;
    template <ui64 Size> using ui8array = array<ui8, Size>;
    template <ui64 Size> using ui16array = array<ui16, Size>;
    template <ui64 Size> using ui32array = array<ui32, Size>;
    template <ui64 Size> using ui64array = array<ui64, Size>;
    template <ui64 Size> using uarray = array<uint_, Size>;
    template <ui64 Size> using i8array = array<i8, Size>;
    template <ui64 Size> using i16array = array<i16, Size>;
    template <ui64 Size> using i32array = array<i32, Size>;
    template <ui64 Size> using i64array = array<i64, Size>;
    template <ui64 Size> using iarray = array<int_, Size>;
    template <ui64 Size> using f32array = array<f32, Size>;
    template <ui64 Size> using f64array = array<f64, Size>;
    template <ui64 Size> using fbigarray = array<fbig, Size>;
    template <ui64 Size> using farray = array<float_, Size>;
    template <ui64 Size> using sarray = array<string, Size>;

    // List Type

    template <class Type> using list = std::list<Type>;

    // Mapping Type

    template <class KeyType, class ValueType> using map = std::map<KeyType, ValueType>;

    // Queue Type

    template <class Type> using queue = std::queue<Type>;

    // Priority Queue Type

    template <class Type> using dpqueue = std::priority_queue<Type, vec<Type>, std::less<Type>>;
    template <class Type> using apqueue = std::priority_queue<Type, vec<Type>, std::greater<Type>>;

    // Tuple Type

    template <class ...Types> using tuple = std::tuple<Types...>;

    // Any Type

    enum class any_type { NONE, UINT, INT, FLOAT, STRING };

    class any {
    private:
        any_type _type;
        union {
            uint_ _uint;
            int_ _int;
            float_ _float;
            wchar_t _string[32];
            ui8 __filler[sizeof(wchar_t) * 32];
        };

    public:
        any() : _type(any_type::NONE) { }
        any(uint_ value) : _type(any_type::UINT), _uint(value) { }
        any(int_ value) : _type(any_type::INT), _int(value) { }
        any(float_ value) : _type(any_type::FLOAT), _float(value) { }
        any(string const& value) : _type(any_type::STRING) {
            std::wcsncpy(_string, value.c_str(), 32);
            _string[31] = L'\0';
        }
        any(any const& value) : _type(value._type) { std::memcpy(__filler, value.__filler, sizeof(__filler)); }
        inline bool get(uint_ & value) {
            if(_type == any_type::UINT) { value = _uint; return true; } else { return false; }
        }
        inline bool get(int_ & value) {
            if(_type == any_type::INT) { value = _int; return true; } else { return false; }
        }
        inline bool get(float_ & value) {
            if(_type == any_type::FLOAT) { value = _float; return true; } else { return false; }
        }
        inline bool get(string & value) {
            if(_type == any_type::STRING) { value = _string; return true; } else { return false; }
        }
        inline void set() {
            _type = any_type::NONE;
            std::memset(__filler, 0x00, sizeof(__filler));
        }
        inline void set(uint_ value) {
            _type = any_type::UINT;
            std::memset(__filler, 0x00, sizeof(__filler));
            _uint = value;
        }
        inline void set(int_ value) {
            _type = any_type::INT;
            std::memset(__filler, 0x00, sizeof(__filler));
            _int = value;
        }
        inline void set(float_ value) {
            _type = any_type::FLOAT;
            std::memset(__filler, 0x00, sizeof(__filler));
            _float = value;
        }
        inline void set(string const& value) {
            _type = any_type::STRING;
            std::memset(__filler, 0x00, sizeof(__filler));
            std::wcsncpy(_string, value.c_str(), 32);
            _string[31] = L'\0';
        }
    };

    // Function Type

    template <class Type> using function = std::function<Type>;

    // Pointer Types

    template <class Type> using ptr = Type*;
    template <class Type> using sptr = std::shared_ptr<Type>;
    template <class Type> using uptr = std::unique_ptr<Type>;
    template <class Type> using wptr = std::weak_ptr<Type>;

    // Positional Types

    using position2 = iarray<2>;
    using position3 = iarray<3>;
    using rect = iarray<4>;
    using box = iarray<6>;

    // Id Types

    using idtype = ui64;
    using sysid = idtype;

    inline constexpr idtype GetId(idtype h, const wchar_t * s) {
        return (*s == 0) ? h :
            GetId((h * 1099511628211ull) ^
                static_cast<idtype>(*s), s + 1);
    }

    /// <summary>
    /// GetId returns a compile-time generated id from a string literal
    /// </summary>
    /// <param name="s">The string to generate an id from</param>
    /// <returns>A 64-bit id number</returns>
    inline constexpr idtype GetId(const wchar_t * s) {
        return true ?
            GetId(14695981039346656037ull, s) :
            throw std::runtime_error("GetId failed!");
    }

    inline constexpr idtype operator "" _id(const wchar_t * s, const std::size_t t) {
        return GetId(s);
    }

    inline constexpr idtype GetId(idtype h, const char * s) {
        return (*s == 0) ? h :
            GetId((h * 1099511628211ull) ^
                static_cast<idtype>(*s), s + 1);
    }

    /// <summary>
    /// GetId returns a compile-time generated id from a string literal
    /// </summary>
    /// <param name="s">The string to generate an id from</param>
    /// <returns>A 64-bit id number</returns>
    inline constexpr idtype GetId(const char * s) {
        return true ?
            GetId(14695981039346656037ull, s) :
            throw std::runtime_error("GetId failed!");
    }

    inline constexpr idtype operator "" _id(const char * s, const std::size_t t) {
        return GetId(s);
    }

    /// <summary>
    /// Id value that represents an invalid id
    /// </summary>
    constexpr idtype InvalidId = L"INVALID"_id;

    constexpr float_ lerp(float_ a, float_ b, float_ beta) {
        return (1.0 - beta) * a + beta * b;
    }
}
